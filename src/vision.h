#ifndef VISION_INCLUDED
#define VISION_INCLUDED
#include "main.h"

class VisionSensor
{
  private:
    pros::Vision* vision;
    pros::vision_signature_s_t red_color;
    int red_id = 1;
    pros::vision_signature_s_t blue_color;
    int blue_id = 2;

  public:
    VisionSensor(pros::Vision* vision_)
    {
        vision = vision_;
        red_color = pros::Vision::signature_from_utility(
          1, 9605, 10241, 9923, -1537, -1171, -1354, 7.7, 0);
        vision->set_signature(red_id, &red_color);
        blue_color = pros::Vision::signature_from_utility(
          2, -3205, -2897, -3051, 7793, 8415, 8104, 8.3, 0);
        vision->set_signature(blue_id, &blue_color);
    }
    ~VisionSensor() { delete vision; }

    int red_on_top()
    {
        vision->clear_led();
        int red_y = vision->get_by_sig(0, 1).top_coord;
        int blue_y = vision->get_by_sig(0, 2).top_coord;
       // pros::lcd::print(
       //   5, "%d, %d / %d", red_y, blue_y, vision->get_object_count());
        if (red_y == 0 || blue_y == 0) {
            return 0;
        } else if (red_y > blue_y) {
            return 1;
        } else {
            return -1;
        }
    }
};
#endif
