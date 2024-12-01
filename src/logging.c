#include "logging.h"
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

int color_enabled = 1;

char LEVEL_TEXT[6][6] = { 
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
};

char* COLOR_RESET = "\033[0;37m";

char LEVEL_COLOR[6][10] = {
        "\033[0;37m", // TRACE
        "\033[0;34m", // DEBUG
        "\033[0;32m", // INFO
        "\033[0;33m", // WARN
        "\033[0;31m", // ERROR
        "\033[0;37m", // FATAL
};

char* getLevelText(enum LOG_LEVEL level) {
        char* rawText = LEVEL_TEXT[ level ];

        if ( !color_enabled ) {
                return rawText;
        }

        char* levelColor = LEVEL_COLOR[ level ];
        size_t levelTextLen = strlen( levelColor ) + strlen( rawText ) + strlen( COLOR_RESET );

        char* levelText = malloc( sizeof( char ) * levelTextLen );

        sprintf( levelText, "%s%s%s", levelColor, rawText, COLOR_RESET );

        return levelText;
}

