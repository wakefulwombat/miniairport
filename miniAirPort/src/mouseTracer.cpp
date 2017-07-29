#include "mouseTracer.h"
#include "input.h"

void MouseTracer::update(){
	if (!this->is_logging) return;

	Vec2D now_pos = Input_T::getOperationInterface_mouse()->getPointerPosition();
	if (now_pos.getDistance(this->world_pos[this->world_pos.size() - 1]) < this->invalid_r) return;
	else this->world_pos.push_back(now_pos);
}

Vec2D MouseTracer::getNextPosition(Vec2D pos, Vec2D vel)
{
	double r = vel.toNorm();
	Vec2D res;


	if (this->world_pos.size() < 2) {
		res = pos + vel;
	}
	else {
		if (this->count >= this->world_pos.size()) {
			res = pos + Vec2D::fromPowAng(vel.toNorm(), (this->world_pos[this->world_pos.size() - 1] - this->world_pos[this->world_pos.size() - 2]).toAngle());
		}
		else {
			if (this->world_pos[this->count + 1].getDistance(pos) < r) {
				double r2 = r - this->world_pos[this->count + 1].getDistance(pos);
				res = this->world_pos[this->count + 1] + Vec2D(r2, (this->world_pos[this->world_pos.size() + 2] - this->world_pos[this->world_pos.size() + 1]).toAngle());
				this->count++;
			}
			else {
				res = pos + Vec2D::fromPowAng(vel.toNorm(), (this->world_pos[this->count + 1] - this->world_pos[this->count]).toAngle());
			}
		}
	}

	return res;
}
