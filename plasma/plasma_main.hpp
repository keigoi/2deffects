#include <memory>
#include "utils/types.h"
#include "utils/plainbmp.h"

typedef uint8_t (*TableCalcFunc)(int x, int y, unsigned w, unsigned h);

std::auto_ptr<syd::PlainBmp> 
prepare_table(unsigned w, unsigned h, TableCalcFunc func);

void 
plasma_draw(syd::Bitmap& bmp, syd::Bitmap& table, unsigned time);

void _BLUR32_(void* in, void* out, int w, int h);
