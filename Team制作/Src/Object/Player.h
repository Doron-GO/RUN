#pragma once
#include <map>
#include <list>
#include <DxLib.h>
#include "ActorBase.h"
#include "../Manager/Camera.h"
#include"../Manager/EffectManager.h"
#include "../Input/Input.h"
#include"Common/CheckPoint.h"
#include<memory>
#include<vector>
class AnimationController;
class Collider;
class Capsule;
class Camera;
class UserInterface;

class Player : public ActorBase
{
public:

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 15.0f;
	static constexpr float SPEED_MOVE_X = 10.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// �W�����v��
	static constexpr float POW_JUMP = 35.0f;

	// �W�����v��t����
	static constexpr float TIME_JUMP_IN = 1.0f;

	static constexpr VECTOR SMOKE_POS = { 0.0f, 0.0f, 0.0f };


	static constexpr VECTOR HIT_POS = { 0.0f, 60.0f, 0.0f };

	static constexpr int HP_MAX = 3;//HP�̍ő�l

	// ���
	enum class P_NUM
	{
		MIN,
		P1,
		P2
	};
	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		CORNER,
		DEAD,
		VICTORY,
		DAMEGE,
		END
	};
	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		POSE
	};

	// �R���X�g���N�^
	Player(Camera& camera,std::vector<CheckPoint> checkPoint,int plyernum);

	// �f�X�g���N�^
	~Player();

	void Init(void) override;
	void Update(Input& input) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(Collider& collider);
	void ClearCollider(void);

	//�ʒu�w��
	void SetPos(VECTOR pos);

	const P_NUM GetPlayerNum()const;

	// �Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const;
	const P_NUM playerNum_;

	//�f�o�b�O�p�֐�
	bool GetColisionCheck();//�����蔻��`�F�b�N���擾
	bool GetVictoryCheck();//�����`�F�b�N���擾

	int GetHp(void);

private:

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	// ��ԊǗ�
	STATE state_;

	// �ړ��X�s�[�h
	float speed_;

	float moveX_ = 0.0f;

	// �ړ�����
	VECTOR moveDir_;

	//���E����
	VECTOR xDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v����
	bool isJump_;

	// �W�����v�̓��͎�t����
	float stepJump_;

	// �Փ˔���ɗp������R���C�_
	std::vector<Collider> colliders_;
	std::shared_ptr< Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �ۉe
	int imgShadow_;

	//�������X�e�[�W�̂ǂ��ɗ����Ă��邩��\���t���[���ԍ�
	int stageFrame_;

	VECTOR initPos_;

	//�|�[�Y�t���O
	bool poseFlag_;

	//HP
	int hp_;

	//�A�j���[�V�����̏�����
	void InitAnimation(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// �X�V�X�e�b�v{
	void UpdateNone(void);
	void UpdatePlay();
	void UpdateCurve();
	//}
	
	
	// �`��n{
	void DrawShadow(void);
	//}
	
	// ����n{
	void ProcessMove();
	void ProcessJump();
	void ProcessPose();
	void ApproachZero(float& variable);
	//���E�ړ�
	bool MoveHorizontally();
	//���E�{�^����������Ă��邩
	bool IsPrassedRight();
	//��󍶃{�^����������Ă��邩
	bool IsPrassedLeft();
	//}

	//�J�[�u�֌W
	void Corner();
	void Curve(CheckPoint::CURVE curve);

	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	//�`�F�b�N�|�C���g�n
	bool IsCheckPoint(CheckPoint c);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// ���n���[�V�����I��
	bool IsEndLanding(void);

	Input input_;
	Camera& camera_;
	std::vector<CheckPoint> checkPoints_;
	std::list<int>const stageModelIds_;

	std::unique_ptr<UserInterface>UI_;

	//�f�o�b�O�p
	bool colisionCheck;//�����蔻��`�F�b�N(��|���S���ł��������true)
	bool PrevcolisionCheck;//�O��̓����蔻��`�F�b�N����

	bool victoryCheck;//�����`�F�b�N
	bool PrevvictoryCheck;//�O��̏����`�F�b�N����


	//�G�t�F�N�g�֘A
	typedef struct
	{
		VECTOR pos_;
		VECTOR rot_;
		VECTOR scl_;

	}EffStat;

	//�G�t�F�N�g�̏��
	std::map<EffectManager::EFF, EffStat> mapEff;
	//�G�t�F�N�g�̏�����
	void InitEffect(void);
	//�G�t�F�N�g�̍X�V
	void UpdateEffect(void);

	void UpdateHitEffect();

	void UpdatePoseEffect(void);
};
