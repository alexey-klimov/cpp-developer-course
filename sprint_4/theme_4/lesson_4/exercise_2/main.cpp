/* Задание 2
 * Строитель Том построил стену. Маляр Билл покрасил её в ваш любимый цвет. Теперь нужен столяр
 * Джек. Он сможет сказать, сколько на эту стену поместится красивых полок для вашей коллекции
 * музыкальных шкатулок. Создайте новый класс Carpenter. Пусть у него будет метод класса, считающий
 * количество полок исходя из того, что на два квадратных метра стены можно повесить одну полку.
 * Назовите этот метод CalcShelves. В качестве параметра он должен получать объект класса Wall.
 *
 */

#include <cassert>

#include "carpenter.h"

int main() {
    Carpenter jack;
    Wall wall(3.5, 2.45);
    assert(jack.CalcShelves(wall) == 4);
}
