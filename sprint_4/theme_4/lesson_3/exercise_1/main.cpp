/* Задание 1
 *
 * При строительстве дома возводят стены. Теперь у вас есть отдельный класс Wall. Посмотрите, как
 * поменялся метод CalcBricksNeeded в классе Builder. Внесите изменения в код. Пусть программа
 * сообщает, сколько кирпичей нужно для стены 3,5 x 2,45 метра.
 */

#include <iostream>

#include "builder.h"

int main() {
    Builder tom;
    double wall_width = 3.5, wall_height = 2.45;
    const auto num_of_bricks = tom.CalcBricksNeeded(Wall(wall_width, wall_height));
    std::cout << num_of_bricks << std::endl;
}
