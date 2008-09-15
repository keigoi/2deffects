#include "utils/window.h"
#include <memory>
#include "utils/fixedframeratekeeper.h"



namespace setup {


std::auto_ptr<syd::Window> create_window();

//void setup();
void sleep();
unsigned long timer();

} // namespace factory