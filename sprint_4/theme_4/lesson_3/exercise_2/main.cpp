/* Задание 2
 *
 * Теперь стены пора красить. Вызовите маляра Билла. Напишите класс Painter, который сможет
 * покрасить стены.
 */

#include "painter.h"
//#include "wall.h"

int main() {
    Painter bill;
    Wall wall(3.5, 2.45);

    bill.Paint(wall, Wall::Color::BLUE);
}
