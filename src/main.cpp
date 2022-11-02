#include "scene.h"
#include "position_component.h"

using std::cout, std::endl;

int main() {
    uint32_t add = 0b101;
    uint32_t sub = 0b010;
    for (uint32_t bit = 0; bit != 8; ++bit)
        cout << !((bit & add) ^ add) << " " << !(bit & sub) << endl;

    ecs::scene s;
    /*

    // make entity
    auto e = s.makeEntity();

    // components
    s.addComponen213t<position>(e, 1, 2); // x = 1, y = 2
    s.addComponen213t<position>(e, 3, 4); // x = 3, y = 4
    s.removeComponent<position>(e);
    s.addComponen213t<position>(e);  // x = 0, y = 0

    // free entity; deletes components too
    s.freeEntity(e);
     */

    return 0;
}
