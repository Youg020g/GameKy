#include "GamePlayScene.h"
#include "../../Actor/Player/Player.h"
#include "../../Field/Field.h"
#include "../../Light/Light.h"
#include "../../Camera/CameraTPS.h"
#include "../../AssetsID/Assets.h"
#include "../../Actor/Enemy/Enemy.h"
#include "../../Actor/EnemyGenerator/EnemyGenerator.h"
#include "../../Actor/Radar/Radar.h"
#include <GSmusic.h>


// �J�n
void GamePlayScene::start() {
    // �����������b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Player, "Assets/model/Kendo/kendo.msh");
    gsLoadSkeleton(Mesh_Player, "Assets/model/Kendo/kendo.skl");
    gsLoadAnimation(Mesh_Player, "Assets/model/Kendo/kendo.anm");
    // ��蕔�����b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Enemy, "Assets/model/Karate/karate.msh");
    gsLoadSkeleton(Mesh_Enemy, "Assets/model/Karate/karate.skl");
    gsLoadAnimation(Mesh_Enemy, "Assets/model/Karate/karate.anm");

    // �X�J�C�{�b�N�X���b�V���̓ǂݍ���
    gsLoadMesh(Mesh_Skybox, "Assets/model/sky/skydome.msh");
    // �Z��I�N�g���[�̓ǂݍ���
    gsLoadOctree(Octree_Koutei, "Assets/model/koutei/koutei.oct");
    // �Z��Փ˔���p�I�N�g���[�̓ǂݍ���
    gsLoadOctree(Octree_KouteiCollider, "Assets/model/koutei/koutei_collider.oct");

    gsLoadTexture(Texture_Number, "Assets/texture/num.png");
    gsLoadTexture(Texture_Text, "Assets/texture/text.png");


    // ���U���g���̔w�i�̓ǂݍ���
    gsLoadTexture(Texture_BlueBack, "Assets/texture/blue.png");
    // �]���^�C�g���̓ǂݍ���
    gsLoadTexture(Texture_Result1, "Assets/texture/result1.png");
    // �]������ �̓ǂݍ���
    gsLoadTexture(Texture_Result2, "Assets/texture/result2.png");
    // ���������摜�̓ǂݍ���
    gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    // �J�n�{�^�������E�̓ǂݍ���
    gsLoadTexture(Texture_Start, "Assets/texture/osu.png");

    // ���[�_�[�̔w�i�̉摜��ǂݍ���
    gsLoadTexture(Texture_Radar, "Assets/texture/radar.png");
    // ���[�_�[�̓_�̉摜��ǂݍ���
    gsLoadTexture(Texture_RadarPoint, "Assets/texture/pt.png");

    // �Q�[���J�n���̌��ʉ��̓ǂݍ���
    gsLoadSE(Se_GameStart, "Assets/sound/Select.wav", 1, GWAVE_DEFAULT);
    // ���������U�������ʉ��̓ǂݍ���
    gsLoadSE(Se_PlayerAttack, "Assets/sound/Attack1.wav", 1, GWAVE_DEFAULT);
    // ���������_���[�W���ʉ��̓ǂݍ���
    gsLoadSE(Se_PlayerDamage, "Assets/sound/Damage2.wav", 1, GWAVE_DEFAULT);
    // ��蕔���_���[�W���ʉ��̓ǂݍ���
    gsLoadSE(Se_EnemeyDamage, "Assets/sound/Attack2.wav", 1, GWAVE_DEFAULT);
    // �^�C���A�E�g�̌��ʉ��̓ǂݍ���
    gsLoadSE(Se_Timeout, "Assets/sound/timeend.wav", 1, GWAVE_DEFAULT);

    // �Q�[���v���C���pBGM�̓ǂݍ���
    gsLoadMusic(Sound_PlayingBGM, "Assets/sound/kendo.wav", GS_TRUE);
    // ���U���g�pBGM�̓ǂݍ���
    gsLoadMusic(Sound_ResultBGM, "Assets/sound/ed.wav", GS_TRUE);

    // BGM�̃o�C���h
    gsBindMusic(Sound_PlayingBGM);
    // BGM�̍Đ�
    gsPlayMusic();
    // �J�n���ʉ��i�������I�̊|�����j
    gsPlaySE(Se_GameStart);


    // �t�B�[���h�N���X�̒ǉ�
    world_.add_field(new Field{ Octree_Koutei, Octree_KouteiCollider, Mesh_Skybox });
    // �J�����N���X�̒ǉ�
    world_.add_camera(new CameraTPS{
              &world_,GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    // ���C�g�N���X�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // ���[�_�[�N���X��ǉ�
    world_.add_actor(new Radar{ &world_ });
    // �v���[���[��ǉ�
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //�G�l�~�[�����N���X
    world_.add_actor(new EnemyGenerator{ &world_});
    // �^�C�}�̏�����
    world_.timer().initialize(60.0f);

    // ���U���g�̏�����
    result_.initialize();
    // �����L���O�t�@�C���̓ǂݍ���
    result_.load("Assets/ranking.txt");
    // ��Ԃ̏�����
    state_ = State::Playing;
    //�@�V�[���I���t���O�̏�����
    is_end_ = false;

}

// �X�V
void GamePlayScene::update(float delta_time) {
    // ��ԕʂ̍X�V
    switch (state_) {
    case State::Playing: update_playing(delta_time); break;
    case State::Result:  update_result(delta_time);  break;
    }

    //// �e�X�g�p��Z�L�[����������G�𐶐�����悤�ɂ���
    //if (gsGetKeyTrigger(GKEY_Z)) {
    //    world_.add_actor(new Enemy{ &world_, GSvector3{0.0f, 0.0f, -20.0f} });
    //}
    
}

// �Q�[���v���C���̍X�V
void GamePlayScene::update_playing(float delta_time) {
    // ���[���h�N���X�̍X�V
    world_.update(delta_time);
    // �Q�[���I�[�o�[�ɂȂ����烊�U���g��ԂɑJ��
    if (world_.is_game_over()) {
        // �^�C���A�E�g�̌��ʉ����Đ�
        gsPlaySE(Se_Timeout);
        // BGM���~
        gsStopMusic();
        // ���U���g�V�[���p��BGM���Đ�
        gsBindMusic(Sound_ResultBGM);
        gsPlayMusic();

        // �_����ǉ�
        result_.add_score(world_.score_.get());
        // ���U���g���^�C�}��������
        result_timer_ = 0.0f;
        // ���U���g��ԂɑJ��
        state_ = State::Result;
    }
}

// ���U���g���̍X�V
void GamePlayScene::update_result(float delta_time) {
    // ���U���g�̍X�V
    result_.update(delta_time);
    // ���U���g���^�C�}�̍X�V
    result_timer_ += delta_time;
    // �Œ�1�b�Ԃ̓��U���g�\���A�X�y�[�X�L�[����������V�[���I��
    if (result_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_SPACE)) {
        is_end_ = true;
    }
}

// �`��
void GamePlayScene::draw() const {
    // ���[���h�̕`��
    world_.draw();
    // ���U���g���̓��U���g��`��
    if (state_ == State::Result) {
        // ���U���g�̕`��
        result_.draw();
        // ���U���g�J�n�P�b��Ɂu�J�n�{�^�������E�I�v��_�ŕ\������
        if (result_timer_ >= 60.0f && fmod(result_timer_, 40.0f) < 20.0f) {
            static const GSvector2 position{ 250, 700 };
            gsDrawSprite2D(Texture_Start, &position, NULL, NULL, NULL, NULL, 0.0f);
        }
    }

    //gsDrawText("���[�V�����ԍ� = %d", world_.count_actor_with_tag("EnemyTag"));
}

// �I�����Ă��邩�H
bool GamePlayScene::is_end() const {
    return is_end_;
}

// ���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
    return "TitleScene";
}

// �I��
void GamePlayScene::end() {
    // BGM�̒�~
    gsStopMusic();
    // �����L���O�t�@�C���̕ۑ�
    result_.save("Assets/ranking.txt");
    // ���[���h������
    world_.clear();
    // �A�Z�b�g�̍폜
    gsDeleteMesh(Mesh_Player);
    gsDeleteSkeleton(Mesh_Player);
    gsDeleteAnimation(Mesh_Player);
    gsDeleteMesh(Mesh_Enemy);
    gsDeleteSkeleton(Mesh_Enemy);
    gsDeleteAnimation(Mesh_Enemy);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteOctree(Octree_Koutei);
    gsDeleteTexture(Texture_Number);
    gsDeleteTexture(Texture_Text);
    gsDeleteTexture(Texture_BlueBack);
    gsDeleteTexture(Texture_Result1);
    gsDeleteTexture(Texture_Result2);
    gsDeleteTexture(Texture_Start);
    gsDeleteTexture(Texture_Radar); 
    gsDeleteTexture(Texture_RadarPoint);
    gsDeleteMusic(Sound_PlayingBGM);
    gsDeleteMusic(Sound_ResultBGM);
    gsDeleteSE(Se_GameStart);
    gsDeleteSE(Se_PlayerAttack);
    gsDeleteSE(Se_PlayerDamage);
    gsDeleteSE(Se_EnemeyDamage);
    gsDeleteSE(Se_Timeout);

}
