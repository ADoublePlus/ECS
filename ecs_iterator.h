#pragma once

#include "ecs_functional.h"
#include "entity_admin.h"

#include <functional>
#include <tuple>

namespace ecs 
{
    template <typename T>
    class ComponentIter
    {
        private:
            class ComponentIterator;

            using RawType = typename details::remove_const_and_reference<T>::type;
            using PointerType = RawType*;
            using Const_PointerType = const PointerType;
            using Iterator = ComponentIterator;
            using Pr = typename std::function<bool(Const_PointerType)>;

            EntityAdmin* _admin;
            Pr _pred;

        public:
            ComponentIter(EntityAdmin* admin) : _admin(admin) {}
            ComponentIter(EntityAdmin* admin, Pr&& pred) : _admin(admin), _pred(std::move(pred)) {}

            Iterator begin() { return Iterator(_admin, _pred, true); }
            Iterator end() { return Iterator(_admin, _pred, false); }

        private:
            class ComponentIterator
            {   
                private:
                    EntityAdmin* _admin;
                    Pr _pred;

                    size_t _cursor;
                    size_t _size;

                public:
                    ComponentIterator(EntityAdmin* admin, Pr& pred, bool is_begin = true) : _admin(admin), _pred(pred), _cursor(0)
                    {
                        _size = _admin->GetAllComponents(details::ComponentIndex::index<T>()).size();

                        if (!is_begin)
                        {
                            _cursor = _size;
                        }

                        find_next();
                    }

                    size_t index() const { return _cursor; }

                    PointerType Component()
                    {
                        ComponentVector& vec = _admin->GetAllComponents(details::ComponentIndex::index<T>());
                        return vec.at(index())->template As<RawType>();
                    }

                    bool operator!=(const ComponentIterator& rhs) const { return !(this->index() == rhs.index()); }
                    PointerType operator*() { return Component(); }

                    ComponentIterator& operator++()
                    {
                        _cursor++;

                        find_next();

                        return *this;
                    }

                private:
                    void find_next()
                    {
                        while (_cursor < _size && _pred && !_pred(Component()))
                        {
                            _cursor++;
                        }
                    }
            };
    };

    template <typename... Args>
    class TupleIter
    {
        private:
            static_assert(sizeof...(Args) != 0, "invalid argument count! need more than one argument!");
            static_assert(sizeof...(Args) != 1, "use ComponentIter<T> to iterate!");

            class TupleIterator;

            using Iterator = TupleIterator;
            using Pr = typename std::function<bool(const typename details::remove_const_and_reference<Args>::type* ...)>;
            using ValueType = std::tuple<typename details::remove_const_and_reference<Args>::type* ...>;
            using FirstArgType = typename std::remove_pointer<typename std::tuple_element<0, ValueType>::type>::type;

            EntityAdmin* _admin;
            Pr _pred;

        public:
            TupleIter(EntityAdmin* admin) : _admin(admin) {}
            TupleIter(EntityAdmin* admin, Pr&& pred) : _admin(admin), _pred(std::move(pred)) {}

            Iterator begin() { return Iterator(_admin, _pred, true); }
            Iterator end() { return Iterator(_admin, _pred, false); }

        private:
            class TupleIterator
            {
                private:
                    EntityAdmin* _admin;
                    Pr _pred;

                    size_t _cursor;
                    size_t _size;

                public:
                    TupleIterator(EntityAdmin* admin, Pr& pred, bool is_begin = true) : _admin(admin), _pred(pred), _cursor(0)
                    {
                        _size = _admin->GetAllComponents(details::ComponentIndex::index<FirstArgType>()).size();

                        if (!is_begin)
                        {
                            _cursor = _size;
                        }

                        find_next();
                    }

                    size_t index() const { return _cursor; }

                    FirstArgType* Component()
                    {
                        ComponentVector& vec = _admin->GetAllComponents(details::ComponentIndex::index<FirstArgType>());
                        return vec.at(index())->template As<FirstArgType>();
                    }

                    bool operator!=(const TupleIterator& rhs) const { return !(this->index() == rhs.index()); }

                    ValueType operator*()
                    {
                        Entity* ent = Component()->Owner();
                        ECS_ASSERT(ent, "Error, TupleIter operator '*' must not be nullptr value");
                        return ValueType(ent->Get<Args...>());
                    }

                    TupleIterator& operator++()
                    {
                        _cursor++;

                        find_next();

                        return *this;
                    }

                private:
                    void find_next()
                    {
                        while (_cursor < _size)
                        {
                            Entity* ent = Component()->Owner();

                            if (ent && ent->Has<Args...>() && (!_pred || (_pred && details::apply(_pred, ent->Get<Args...>()))))
                                break;

                            _cursor++;
                        }
                    }
            };
    };
}