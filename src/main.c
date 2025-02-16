#include "window.h"
#include "logging.h"

int
main(int argc, char* argv[])
{
        Window*         window = WindowCreate(1200, 675, 60);

        init(window);

        run(window);

        log_info("application exiting...");

        return 0;
}
