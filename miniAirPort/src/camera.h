#pragma once

#include "common.h"
#include "base_objectBase.h"
#include <vector>
#include <string>
#include <memory>

class ImagePropertyData;

class CameraDrawInterface {
public:
	virtual ~CameraDrawInterface() {}

	virtual void drawImageInWorld(Vec2D center_pos_world, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) = 0;
	virtual void drawImageInWorld(Vec2D center_pos_world, Vec2D img_center, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) = 0;
	virtual void drawImageOnWindow(Vec2D center_pos_window, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) = 0;
	virtual void drawImageOnWindow(Vec2D center_pos_window, Vec2D img_center, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) = 0;

	//�}�`�`��p
	virtual void drawSquareOnWindowFixed(Vec2D center_pos_window, Size size, Color_RGB color, bool fill = true) = 0;
	virtual void drawSquareOnWindowSwayed(Vec2D center_pos_window, Size size, Color_RGB color, bool fill = true) = 0;
	virtual void drawRotateSquareInWorld(Vec2D center_pos_world, Size original_size, double rot_angle, Color_RGB color, bool fill = true) = 0;
	virtual void drawRotateSquareOnWindowFixed(Vec2D center_pos_window, Size size, double rot_angle, Color_RGB color, bool fill = true) = 0;
	virtual void drawRotateSquareOnWindowSwayed(Vec2D center_pos_window, Size size, double rot_angle, Color_RGB color, bool fill = true) = 0;
	virtual void drawLineInWorld(Vec2D pos_start_world, Vec2D pos_end_world, Color_RGB color, int width = 1) = 0;
	virtual void drawLineOnWindowFixed(Vec2D pos_start_window, Vec2D pos_end_window, Color_RGB color, int width = 1) = 0;
	virtual void drawLineOnWindowSwayed(Vec2D pos_start_window, Vec2D pos_end_window, Color_RGB color, int width = 1) = 0;
	virtual void drawCircleInWorld(Vec2D center_pos_world, int r_original, Color_RGB color, bool fill = true) = 0;
	virtual void drawCircleOnWindowFixed(Vec2D center_pos_window, int r, Color_RGB color, bool fill = true) = 0;
	virtual void drawCircleOnWindowSwayed(Vec2D center_pos_window, int r, Color_RGB color, bool fill = true) = 0;
	virtual void drawStringInWorld(Vec2D center_pos_world, std::string text, Color_RGB color, double expansion = 1.0) = 0;
	virtual void drawStringOnWindowFixed(Vec2D center_pos_window, std::string text, Color_RGB color, double expansion = 1.0) = 0;
	virtual void drawStringOnWindowSwayed(Vec2D center_pos_window, std::string text, Color_RGB color, double expansion = 1.0) = 0;
};

class Camera : public CameraDrawInterface{
private:
	Size windowPixelSize, worldSize;
	double shield_ratio;//�`��Օ���(0.0:�� �` 1.0:�S�`��)
	
	Vec2D pos_anchor_window;//�J������_�̃X�N���[�����W

	int move_count, move_count_max;
	Vec2D pos_anchor_world_now;//�J������_�̃��[���h���W
	Vec2D anchor_target_world_pos_start, anchor_target_world_pos_goal;

	int zoom_count, zoom_count_max;
	double zoom_magnification_now;
	double zoom_magnification_start, zoom_magnification_end;

	int rotate_count, rotate_count_max;
	double rotate_angle_now;
	double rotate_angle_start, rotate_angle_end;

	int sway_count, sway_count_max, sway_update_time;
	Vec2D sway_now;
	int sway_power_max;
	double sway_vib_trans_rad;
	int sway_vib_rotate_freq;

	void init(Size window_size);
	
	bool isVisibleForWindowPos(Vec2D pos_window);//�E�B���h�E���W�n[px]�̓_���E�B���h�E���ɑ��݂��邩
	bool isVisibleForWindowPos(Vec2D pos_window, Size size);//�E�B���h�E���W�n[px]�̋�`���E�B���h�E����(�ꕔ�ł�)���݂��邩(��]���l�����Ȃ�)
	bool isVisibleForWindowPos(Vec2D pos_window, Size size, double rot_angle_in_window);//�E�B���h�E���W�n[px]�̋�`���E�B���h�E����(�ꕔ�ł�)���݂��邩(�E�B���h�E���W�n�ł̉�]�p�x���l������)

public:
	Camera(Size window_size) { this->init(window_size); this->worldSize = window_size; }
	Camera(Size window_size, Size world_size) { this->init(window_size); this->worldSize = world_size; }
	Camera(Size window_size, Vec2D pos_anchor_world, Vec2D pos_anchor_window, Size world_size) {
		this->init(window_size); this->pos_anchor_world_now = pos_anchor_world; this->pos_anchor_window = pos_anchor_window; this->worldSize = world_size; this->anchor_target_world_pos_start = pos_anchor_world; this->anchor_target_world_pos_goal = pos_anchor_world;
	}
	
	void setShieldRatio(double ratio) { this->shield_ratio = ratio; }

	void setAnchorWorldPosition(Vec2D world_pos);
	void setAnchorWorldPosition(Vec2D world_pos, int count);
	void setAnchorWindowPosition(Vec2D window_pos);
	void setAnchorWindowPosition(Vec2D window_pos, int count);

	void setZoom(double zoom);
	void setZoom(double zoom, int count);
	void setZoomAdd(double zoom_add, int count);
	void setZoomReset(int count);

	void setRotation(double angle_rad);
	void setRotation(double angle_rad, int count);
	void setRotationAdd(double angle_rad_add, int count);

	void setSwayRandomInSquare(int pow, int count_max, int update_time);
	void setSwayVibrationDecaying(int pow, int count_max, double trans_rad, int rot_freq);


	//�摜�`��p
	void drawImageInWorld(Vec2D center_pos_world, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) override;
	void drawImageInWorld(Vec2D center_pos_world, Vec2D img_center, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) override;
	void drawImageOnWindow(Vec2D center_pos_window, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) override;
	void drawImageOnWindow(Vec2D center_pos_window, Vec2D img_center, int graphHandle, const std::shared_ptr<ImagePropertyData> &img_prop) override;
	
	//�}�`�`��p
	void drawSquareOnWindowFixed(Vec2D center_pos_window, Size size, Color_RGB color, bool fill = true) override;//not sway, not rotate
	void drawSquareOnWindowSwayed(Vec2D center_pos_window, Size size, Color_RGB color, bool fill = true) override;//sway, not rotate
	void drawRotateSquareInWorld(Vec2D center_pos_world, Size original_size, double rot_angle, Color_RGB color, bool fill = true) override;
	void drawRotateSquareOnWindowFixed(Vec2D center_pos_window, Size size, double rot_angle, Color_RGB color, bool fill = true) override;//not sway, rotate (�J�����ɂ���]�͍l���Ȃ��B�}�`�̉�]�̂�)
	void drawRotateSquareOnWindowSwayed(Vec2D center_pos_window, Size size, double rot_angle, Color_RGB color, bool fill = true) override;//sway, rotate (�J�����ɂ���]�͍l���Ȃ�.�}�`�̉�]�̂�)
	void drawLineInWorld(Vec2D pos_start_world, Vec2D pos_end_world, Color_RGB color, int width = 1) override;
	void drawLineOnWindowFixed(Vec2D pos_start_window, Vec2D pos_end_window, Color_RGB color, int width = 1) override;
	void drawLineOnWindowSwayed(Vec2D pos_start_window, Vec2D pos_end_window, Color_RGB color, int width = 1) override;
	void drawCircleInWorld(Vec2D center_pos_world, int r_original, Color_RGB color, bool fill = true) override;
	void drawCircleOnWindowFixed(Vec2D center_pos_window, int r, Color_RGB color, bool fill = true) override;
	void drawCircleOnWindowSwayed(Vec2D center_pos_window, int r, Color_RGB color, bool fill = true) override;
	void drawStringInWorld(Vec2D center_pos_world, std::string text, Color_RGB color, double expansion = 1.0) override;
	void drawStringOnWindowFixed(Vec2D center_pos_window, std::string text, Color_RGB color, double expansion = 1.0) override;
	void drawStringOnWindowSwayed(Vec2D center_pos_window, std::string text, Color_RGB color, double expansion = 1.0) override;

	void update();

	Size getWorldSize() { return this->worldSize; }

	Vec2D toWindowPosPxFromWorldPos(Vec2D world_pos) { return (this->pos_anchor_window + Mat2D::rotation(-this->rotate_angle_now, (world_pos - this->pos_anchor_world_now)*this->zoom_magnification_now)); }
	Vec2D toWorldPosFromWindowPosPx(Vec2D window_pos) { return (this->pos_anchor_world_now + Mat2D::rotation(this->rotate_angle_now, (window_pos - this->pos_anchor_window) / this->zoom_magnification_now)); }
};
