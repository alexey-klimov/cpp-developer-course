#include "carpenter.h"
#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall) {
    return CalcSquare(wall.GetWidth(), wall.GetHeight()) / 2;
}
