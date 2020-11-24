#ifndef PCC_MOTOR_H

#define KP 0.1
#define r_sen_ref 555
#define l_sen_ref 555
#define r_wall_ref 55
#define l_wall_ref 55
//#define fr_wall_ref 122
//#define fl_wall_ref 122

void init_motor(void);
void int_mot_r(void);
void int_mot_l(void);
void int_cmt1(void);
void kasoku(float accel);
void gensoku(float accel);
void step(void);
void rotate180(void);
#endif
