/* Importe tout les .h */

#ifndef SRC_INCLUDES_IMPORTS_H_
#define SRC_INCLUDES_IMPORTS_H_

// On inclut les .h de la librairie standard
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

// On inclut les .h de notre application
#include "./SoS.h"
#include "./intermediate/actions.h"
#include "./intermediate/operands.h"
#include "./intermediate/operators.h"
#include "./intermediate/ctrl_ql.h"
#include "./intermediate/quads.h"
#include "./intermediate/symbols.h"
#include "./mips/code.h"
#include "./mips/code_quads.h"
#include "./mips/code_utils.h"
#include "./mips/data.h"
#include "./mips/mips.h"
#include "./utils.h"
#include "./intermediate/for_struct.h"

#endif // SRC_INCLUDES_IMPORTS_H_
