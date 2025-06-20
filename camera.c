#include "camera.h"
#include "motor.h"
#include "stage_env.h"
#include "system.h"
#include "outputPicture.h"

#include "syssvc/serial.h"

#include <math.h>
#include <stdlib.h> 

#include <stdio.h>
#include <string.h>

int g_detected_flag = 0;

void search_task(ITEMS *obj)
{
    IMAGE *img = alloc_img(IMAGE_WIDTH, IMAGE_HEIGHT, 3);
    int count = 0;    
    
    do { 
        g_detected_flag = 0;

        cap_picture(img);
        int res = detect_ball(img, obj->robot, obj->ball);
        
        snprintf (str, sizeof(str), "%4d", res);
        ev3_lcd_draw_string (str, 0, l_font_property.height * 5);

        if (res == 0) {
            break;
        }
        else if (res == 1) {
            robot_rotate(obj->robot, 90);
        }
        else if (res == 2) {
            robot_rotate(obj->robot, 45);
        }
        else if (res == 3) {
            robot_rotate(obj->robot, -45);
        }

        count++;
    }
    while (count < 4);

    // calc_hidden_area();  // if robot get environment

    free_img(img);
}

void cap_picture(IMAGE *img)
{
	intptr_t rcvdtq;
	intptr_t senddtq;
	Cam_SendQueue sendQueue;
	Cam_ResvQueue *picData;

    ev3_lcd_draw_string("Take Picture", 0, l_font_property.height*2);
    
    sendQueue.order = CAM_ORDER_TAKE_PICTURE;
    senddtq = (uint32_t)(&sendQueue);
    snd_dtq(CAM_EXECUTE_DTQ, senddtq);
    tslp_tsk(500);
    
    while (1) {
        if (prcv_dtq(CAM_RESULT_DTQ, &rcvdtq) == E_OK) {
            picData = (Cam_ResvQueue*)rcvdtq;
         
            if (picData->type == CAM_RES_TAKE_PICTURE) {
                if (picData->result == true) {
                    tslp_tsk(500);

                    for (int k = 0; k < IMAGE_HEIGHT; k++) {
                        for (int j = IMAGE_WIDTH - 1; j >= 0; j--) {
                            int value[3] = {0, 0, 0};
                            int h = 0;

                            int r, g, b;

                            value[0] = (int)(picData->picture.rgb.rgbData[(j + k * IMAGE_WIDTH) * 3 + 0]);
                            value[1] = (int)(picData->picture.rgb.rgbData[(j + k * IMAGE_WIDTH) * 3 + 1]);
                            value[2] = (int)(picData->picture.rgb.rgbData[(j + k * IMAGE_WIDTH) * 3 + 2]);
                            
                            r = value[0];
                            g = value[1];
                            b = value[2];

                            for (int i = 0; i < 2; i++) {
                                for (int l = i + 1; l < 3; l++) {
                                    if (value[i] > value[l]) {
                                        int tmp;
                                        tmp = value[i];
                                        value[i] = value[l];
                                        value[l] = tmp;
                                    }
                                }
                            }

                            if (value[0] == value[2]) {
                                img->data[(j + k * img->width) * 3 + 0] = 0;
                                img->data[(j + k * img->width) * 3 + 1] = 0;
                                img->data[(j + k * img->width) * 3 + 2] = (uchar)value[0];
                                continue;
                            }
                            else if (value[2] == r) {
                                h = (60 * (g - b) / (value[2] - value[0])) * 255 / 360;
                            }
                            else if (value[2] == g) {
                                h = (60 * (g - b) / (value[2] - value[0]) + 120) * 255 / 360;
                            }
                            else if (value[2] == g) {
                                h = (60 * (g - b) / (value[2] - value[0]) + 240) * 255 / 360;
                            }
                            if (h < 0) h += 255;

                            img->data[(j + k * img->width) * 3 + 0] = (char)h;
                            img->data[(j + k * img->width) * 3 + 1] = (char)(value[2] - (double)value[0] / (double)value[2] * 255);
                            img->data[(j + k * img->width) * 3 + 2] = (char)value[2];
                         }
                    }
                    break;
                }
            }
        }
        tslp_tsk(10);
    }

    bool outputBmpRes = outputBmpFile("/test.bmp", picData->picture.rgb.rgbData);

    ev3_lcd_draw_string("Get image data", 0, l_font_property.height*2);
}

IMAGE *alloc_img(int width, int height, int channel)
{
    IMAGE *img = (IMAGE *)malloc(sizeof(IMAGE));

    img->width = width;
    img->height = height;

    img->data = (uchar *)malloc(sizeof(uchar) * width * height * channel);

    return img;
}

void free_img(IMAGE *img)
{
    free(img->data);
    free(img);
}

int detect_ball(IMAGE *img, ROBOT *robot, OBJECT *ball)
{
    int max_idx = 0;
    int ball_center_x = 0;
    int degree;
    int distance;
    double mmeter_per_px;

    g_detected_flag = 0;
    IMAGE *ball_img = alloc_img(img->width, img->height, 1);

    int ball_area_x[IMAGE_HEIGHT];

    for (int k = 0; k < img->height; k++) {
        ball_area_x[k] = 0;
        for (int j = 0; j < img->width; j++) {

            if (extract_color(&img->data[(j + k * img->width) * 3])) {   // count ball width //
                if (j == 0) {
                    return 2;
                }
                else if (j == img->width - 1) {
                    return 3;
                }
                
                ball_img->data[j + k * ball_img->width] = 255;
                ball_area_x[k]++;
            }
        }
    }
    if (g_detected_flag == 0) return 1;

    for (int i = 1; i < IMAGE_HEIGHT; i++) {    // search range of ball //
        if (ball_area_x[max_idx] < ball_area_x[i]) {
            max_idx = i;
        }
    }

    for(int j = 0; j < img->width; j++) {   // calc ball position x //
        if (img->data[(j + max_idx * img->width)] == 255) {
            ball_center_x = (j + ball_area_x[max_idx] / 2) - 80;
            break;
        }
    }

    degree = calc_target_direction(ball_center_x);

    mmeter_per_px = BALL_SIZE * 10.0 / ball_area_x[max_idx];
    distance = calc_target_distance(mmeter_per_px, ball_center_x, degree);

    ball->x = robot->x + distance * sin(robot->direct - theta);
    ball->y = robot->y + distance * cos(robot->direct - theta);

    free_img(ball_img);
    return 0;
}

int extract_color(uchar *value)
{
    uchar ret = 0;

    // // BGR to brightness    //
    // int v = 0.114 * (*value) + 0.587 * (*(value + 1)) + 0.299 * (*(value + 2));
    // if (v > 200) {
    //     ret = 255;
    //     g_detected_flag = 1;
    // }
    
    // // RGB    //
    // if (*(value + 2) <= 235) {}
    // else if (*(value + 1) < 90 && *(value + 1) > 120) {}
    // else if (*value < 165 && *value > 195) {}
    // else {
    //     ret = 255;
    //     g_detected_flag = 1;
    // }

    // HSV  //
    if (*value < 40 || *value > 230) {
        if (*(value + 1) > 100) {
            if (*(value + 2) > 80) ret = 255;
        }
    }

    return ret;
}

int calc_target_direction(int ball_center_x)
{
    int res = asin(sin(PI * 60 / 180) * ball_center_x / 80);

    return res;
}

int calc_target_distance(double mmeter_per_px, int ball_center_x, int degree)
{
    int range;
    int res;

    range = ball_center_x * mmeter_per_px / sin(PI * degree / 180);
    res = range * tan(PI * degree / 180);

    return res;
}