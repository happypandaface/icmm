#ifndef ANIMATION_H_
#define ANIMATION_H_

#define ANM_NONE 0
#define ANM_GAS_BALL 1
#define ANM_PUNCH 2

struct Frame;

struct Frame
{
	int texture;
	int isLastFrame;
	float frameTime;
	struct Frame* nextFrame;
};

typedef struct
{
	struct Frame* firstFrame;
	struct Frame* lastFrame;
}Animation;

Animation* anm_gas_ball;
Animation* anm_hand_punch;

typedef struct
{
	int timesLooped;
	float currentTime;
	struct Frame* currentFrame;
}AnimationInstance;

void setup_animations();
void new_animation(Animation* anim);
void add_frame(Animation* anim, int texture, float frameTime);
void step_animation(AnimationInstance* animInst, float dt);
void set_instance(Animation* anim, AnimationInstance* animInst);
void set_animation(AnimationInstance** anim, int anim_type);
void unload_animation(AnimationInstance** anim);

#endif