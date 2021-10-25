#include "GamePlayScene.h"
#include "../../Actor/Player/Player.h"
#include "../../Field/Field.h"
#include "../../Light/Light.h"
#include "../../Camera/CameraTPS.h"
#include "../../AssetsID/Assets.h"
#include "../../Actor/Enemy/Enemy.h"

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

    // �t�B�[���h�N���X�̒ǉ�
    world_.add_field(new Field{ Octree_Koutei, Octree_KouteiCollider, Mesh_Skybox });
    // �J�����N���X�̒ǉ�
    world_.add_camera(new CameraTPS{
              &world_,GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    // ���C�g�N���X�̒ǉ�
    world_.add_light(new Light{ &world_ });
    // �v���[���[��ǉ�
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
}

// �X�V
void GamePlayScene::update(float delta_time) {

    // �e�X�g�p��Z�L�[����������G�𐶐�����悤�ɂ���
    if (gsGetKeyTrigger(GKEY_Z)) {
        world_.add_actor(new Enemy{ &world_, GSvector3{0.0f, 0.0f, 20.0f} });
    }

    world_.update(delta_time);
}

// �`��
void GamePlayScene::draw() const {
    world_.draw();
}

// �I�����Ă��邩�H
bool GamePlayScene::is_end() const {
    return false;
}

// ���̃V�[����Ԃ�
std::string GamePlayScene::next() const {
    return "TitleScene";
}

// �I��
void GamePlayScene::end() {
    // ���[���h������
    world_.clear();
}