#include "scene.h"
#include "bitmask.h"

using std::cout, std::endl;

struct position : public ecs::component<position> {
    uint32_t x, y;

    position() = default;
    explicit position(uint32_t t_x, uint32_t t_y) : x(t_x), y(t_y) {}
};

struct health : public ecs::component<health> {
    uint32_t hp;

    health() = default;
    explicit health(uint32_t t_hp) : hp(t_hp) {}
};

int main() {
//    uint32_t mask_and = 0b101; // include if both
//    uint32_t mask_not = 0b010; // exclude second index
//    for (uint32_t bit = 0; bit != 8; ++bit) {
//        cout << (bit >> 2 & 1) << (bit >> 1 & 1) << (bit & 1) << " ";
//        cout << !((bit & mask_and) ^ mask_and) << " " << !(bit & mask_not) << endl;
//    }
//    ecs::scene s;

    // make entity
    // auto e = s.makeEntity();


    ecs::scene level_1;

    // create one instance
    ecs::entity player = level_1.make_entity();
    level_1.mutate(player).set_archetype<position>(1, 2);

    // add one component
//    level_1.mutate(player).add_component<health>(10);
//
//    // batch create instance
//    ecs::entity_batch batch = level_1.make_entity(10000);
//    level_1.mutate(batch).set_archetype<position, health>(1, 2, 10);
//
//    // remove one component
//    level_1.mutate(batch).remove_component<health>();
//
//    // delete entity
//    level_1.free_entity(player);

    return 0;
}
