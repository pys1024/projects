#include "MyTest.h"

#define ITEM_HEIGHT_MIN   100
#define ITEM_PAD          ((LV_VER_RES - ITEM_HEIGHT_MIN) / 2)

using namespace Page;

MyTest::MyTest()
{
}

MyTest::~MyTest()
{

}

void MyTest::onViewLoad()
{

	lv_obj_remove_style_all(root);
	lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_style_bg_color(root, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);
	// lv_obj_set_style_pad_ver(root, ITEM_PAD, 0);
	// lv_obj_set_size(root, 240, 240);
	// lv_obj_set_pos(root, 0, 0);

	// lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
	// lv_obj_set_flex_align(
	// 	root,
	// 	LV_FLEX_ALIGN_START,
	// 	LV_FLEX_ALIGN_START,
	// 	LV_FLEX_ALIGN_CENTER
	// );

	lv_obj_t* cont = lv_obj_create(root);
	lv_obj_remove_style_all(cont);
	lv_obj_set_style_bg_color(cont, lv_color_white(), 0);
	lv_obj_set_size(cont, 50, 50);
	// lv_obj_set_pos(cont, 0, 0);
	lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_center(cont);


    // lv_point_t points[] = { {5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10} };
    static lv_point_t points[] = { {0, 0}, {10, 0}, {10, 10}, {0, 10} };

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 1);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t *line;
    line = lv_line_create(cont);
    lv_line_set_points(line, points, sizeof(points) / sizeof(lv_point_t));
    // lv_obj_add_style(line, &style_line, 0);
	// lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
    lv_obj_set_size(line, 20, 20);
    lv_obj_set_pos(line, 0, 0);
    // lv_obj_center(line);

    lv_obj_set_style_line_width(line, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(line, lv_color_hex(0x08ea21), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(line, LV_OPA_COVER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(line, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    // lv_obj_update_layout(lv_scr_act());
    
    return;

    lv_obj_t * label1 = lv_label_create(root);
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
                      "and wrap long text automatically.");
    lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
}