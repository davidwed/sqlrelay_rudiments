// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/regularexpression.h>
#include <stdio.h>

int main(int argv, const char **argc) {

        // A quick match...
        if (regularexpression::match("Hello Dave!",".*Dave.*")) {
                printf("The string contains .*Dave.*\n");
        }

        // If you need to match over and over...
        regularexpression       re(".*Dave.*");
        if (re.match("Hello Dave!")) {
                printf("The string contains .*Dave.*\n");
        }
        if (re.match("Goodbye Dave!")) {
                printf("The string contains .*Dave.*\n");
        }
        if (re.match("Dave is a jerk!")) {
                printf("The string contains .*Dave.*\n");
        }
        if (re.match("Dave writes cool software!")) {
                printf("The string contains .*Dave.*\n");
        }
        if (re.match("See ya later Dave!")) {
                printf("The string contains .*Dave.*\n");
        }
        
}
