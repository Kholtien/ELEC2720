/* Revision Information
 * ====================
 *
 * $Id: my_main.h 4826 2021-03-22 04:32:18Z rbetz $
 * $Author: rbetz $
 * $Date: 2021-03-22 15:32:18 +1100 (Mon, 22 Mar 2021) $
 * $Revision: 4826 $
 */

/*
 * Created on: Monday March 15, 2021
 * Created by: Robert Betz
 */

#ifndef INC_MY_MAIN_H_
#define INC_MY_MAIN_H_

#define GREEN_LED LD4_Pin
#define ORANGE_LED LD3_Pin
#define RED_LED LD5_Pin
#define BLUE_LED LD6_Pin



extern void fvdMyMain(UART_HandleTypeDef *pstHuart2);

#endif /* INC_MY_MAIN_H_ */
