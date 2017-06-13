#ifndef MODESTARSCAPE
#define MODESTARSCAPE

#include "cocos2d.h"
#include "GameManager.h"
#include <ui/UIButton.h>
#include "Starback.h"
#include "Segbar.h"

struct StarWrap
{
	Sprite *sprite;
	Label *m_label;
	int m_x;
	int m_y;
};

class Path
{
	bool Long_gen(Node *o, Star ***stars, int width, int height, Vec2 origin, Vec2 destination);
	bool Short_gen(Node *o, Star ***stars, int width, int height, Vec2 origin, Vec2 destination);
public:
	Sprite *m_sprites[8];
	Vec2 m_positions[8];
	int m_index;
	Path();
	~Path();

	void Clear();
	void Increment();
	bool Generate(Node *o, Star ***stars, int width, int height, Vec2 origin, Vec2 destination);
	Vec2 getStep();
};

class Mode_Starscape : public Layer
{
	Sprite *m_player;

	Node *m_root;

	StarWrap *m_stars[16][16];

	bool m_slide;

	void Playerpos();
	bool updatePlayer();
	
	Path m_path;

	cocos2d::ui::Button *m_buttons[6];
	cocos2d::Label *m_labels[4];
	Segbar *m_bars[2];
	int m_costtime, m_costfuel;

	void CalcCost(int x, int y);

	void SetupPbar();

	Starback *m_Background;
	Starback *m_Midground;

	void WriteDate();
public:


	~Mode_Starscape();
	static cocos2d::Scene* createScene(bool solarview=false);

	void Starspawn();

		void Starcull();

	virtual bool init();

	CREATE_FUNC(Mode_Starscape);

	void 	onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);

	void Startouched(int x, int y);

	void ButtonCallback0(Ref *pSender);

	void ButtonCallback1(Ref *pSender);

	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);
	void ButtonCallback4(Ref *pSender);
	void ButtonCallback5(Ref *pSender);

	void Concluded();

	void SetLabels();

	void GenLogDisplay();
	void TimeIncrement(int time);
};


#endif