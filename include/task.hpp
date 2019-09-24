/**
    @file task class implementation
*/

#ifndef NOZ_TASK_HPP
#define NOZ_TASK_HPP

#include <list>
#include <memory>
#include <optional>

namespace Noz
{
    /**
        @class task
        @brief The task class represents task system.
               All task been managed by global task list as std::shared_ptr.
               The task class provides detach function for detaching task itself from global task list in linear time.
    */
    class task : public std::enable_shared_from_this<task>
    {
    public:
        virtual ~task() {}

        class detacher
        {
        public:
            detacher()
            {}

            template<typename type>
            void attach(
                std::list<std::shared_ptr<type>> * list_ptr,
                const typename std::list<std::shared_ptr<type>>::iterator & iter)
            {
                detacher_list.push_back(std::make_unique<detacher_impl<type>>(list_ptr, iter));
            }

            template<typename type>
            void detach(std::list<std::shared_ptr<type>> * list_ptr)
            {
                detacher_list.remove_if(
                    [=](auto & obj) -> bool {
                        return obj.list_ptr == list_ptr;
                    }
                );
            }

            void detach_all()
            {
                for (auto & detacher : detacher_list)
                {
                    detacher->detach();
                }
                detacher_list.clear();
            }

        private:
            class detacher_interface
            {
            public:
                ~detacher_interface()
                {
                }

                virtual void detach() = 0;
            };

            template<typename type>
            class detacher_impl
                : public detacher_interface
            {
            public:
                detacher_impl(
                    std::list<std::shared_ptr<type>> * list_ptr,
                    const typename std::list<std::shared_ptr<type>>::iterator & iter
                )
                    : list_ptr{list_ptr}
                    , opt_iter{iter}
                {}

                ~detacher_impl()
                {
                    detach();
                }

                void detach() override
                {
                    if (list_ptr)
                    {
                        list_ptr->erase(*opt_iter);
                        list_ptr = nullptr;
                        opt_iter = std::nullopt;
                    }
                }

            private:
                std::list<std::shared_ptr<type>> * list_ptr;
                std::optional<typename std::list<std::shared_ptr<type>>::iterator> opt_iter;
            };

            std::list<std::unique_ptr<detacher_interface>> detacher_list;
        }; // detacher

        template<typename type, typename function_type>
        static void for_each(function_type && function)
        {
            for (auto && ptr : global_list<type>())
            {
                function(*ptr);
            }
            std::remove_if(global_list<type>().begin(), global_list<type>().end(), [](auto && obj) -> bool {
                obj->detacher_.detach_all();
                return obj->detached;
            });
        }

        /**
            Create task and append to global task list.
            @param args [in] arguments for cunstructor of type
        */
        template<typename type, typename ... args_t>
        static std::shared_ptr<type> create(args_t && ... args)
        {
            auto ptr = std::make_shared<type>(std::forward<args_t>(args) ...);
            ptr->on_create();
            return ptr;
        }

        virtual void on_create()
        {
            attach<task>();
        }

        template<typename type>
        void attach()
        {
            auto & list = global_list<type>();
            auto ptr = shared_from_this();
            list.push_back(std::dynamic_pointer_cast<type>(ptr));
            detacher_.attach<type>(std::addressof(list), std::prev(list.end()));
        }

        template<typename type>
        void detach()
        {
            detached = true;
        }
               
    private:
        template<typename type>
        static std::list<std::shared_ptr<type>> & global_list()
        {
            static std::list<std::shared_ptr<type>> list;
            return list;
        }

        detacher detacher_;
        bool detached{};
    };

    class updatable
        : public virtual task
    {
    public:
        updatable()
        {}

        virtual ~updatable() {}

        virtual void update() = 0;
    };

    /**
        @interface drawable
        @brief The drawable interface represents that inheritance class implments void draw() function.
    */
    class drawable
        : public virtual task
    {
    public:
        drawable()
        {}

        virtual ~drawable() {}

        virtual void draw() = 0;
    };

    class object
        : public virtual drawable
        , public virtual updatable
    {
    public:
        object()
        {}

        virtual ~object() {}

        void on_create() override
        {
            attach<task>();
            attach<updatable>();
            attach<drawable>();
            attach<object>();
        }
    };

}

#endif // NOZ_TASK_HPP
