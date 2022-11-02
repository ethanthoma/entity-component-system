//
// Created by User on 2022-01-02.
//

#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

namespace ecs {
    typedef uint32_t component_id;

    class component_id_manager {
    private:
        static component_id next_id_;
    protected:
        template<typename T> static component_id get_component_id() {
            static const component_id id = next_id_++;
            return id;
        }
    };

    component_id component_id_manager::next_id_ = 0;

    template<typename T>
    class component : private component_id_manager {
    public:
        static component_id get_id() {
            return get_component_id<T>();
        }
    };
}

#endif //ECS_COMPONENT_H
