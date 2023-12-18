/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    colors.h
 * @Author  Dorian Dalbin
 * @Created	2023-08-03
 * @brief
 *
 **/
#ifndef INC_COLORS_H_
#define INC_COLORS_H_

#define COLOR_(code) "\033[1;" code "m"
#define COLOR_BLACK   COLOR_("30")
#define COLOR_RED     COLOR_("31")
#define COLOR_GREEN   COLOR_("32")
#define COLOR_YELLOW  COLOR_("33")
#define COLOR_BLUE    COLOR_("34")
#define COLOR_MAGENTA COLOR_("35")
#define COLOR_CYAN    COLOR_("36")
#define COLOR_WHITE   COLOR_("37")
#define COLOR_DEFAULT COLOR_("0")

#endif /* INC_COLORS_H_ */
