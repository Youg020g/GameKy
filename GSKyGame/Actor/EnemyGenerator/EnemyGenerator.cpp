#include "EnemyGenerator.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../Enemy/Enemy.h"

// �R���X�g���N�^
EnemyGenerator::EnemyGenerator(IWorld* world) {
    world_ = world;
    tag_ = "Generator";
    name_ = "EnemyGenerator";
    timer_ = time_;
    enable_collider_ = false;	// ���G��Ԃɂ���
}

// �X�V
void EnemyGenerator::update(float delta_time) {
    GSvector3 position{0.0f,0.0f,0.0f};
    int pos_rand = gsRand(0, 3);
    // 0.5�b�`2.0�b�̃����_���ȊԊu�œG�𐶐�����
    if (timer_ < 0.0f) {
        if (pos_rand == 0) {
            position = {
                gsRandf(world_->field()->ground_min.x, world_->field()->ground_max.x),
                0.0f,
                 world_->field()->ground_min.z };
        }
        if (pos_rand == 1) {
            position = {
          gsRandf(world_->field()->ground_min.x, world_->field()->ground_max.x),
           0.0f,
           world_->field()->ground_max.z };
        }
        if (pos_rand == 2) {
            position = {
           world_->field()->ground_min.x,
           0.0f,
           gsRandf(world_->field()->ground_min.z,world_->field()->ground_max.z) };
        }
        if (pos_rand == 3) {
            position = {
           world_->field()->ground_max.x,
           0.0f,
           gsRandf(world_->field()->ground_min.z,world_->field()->ground_max.z) };
        }

        // �G�L�����N�^�𐶐�����      
        if(world_->count_actor_with_tag("EnemyTag") < enemy_num_max_) world_->add_actor(new Enemy{ world_, position });
        //�^�C�}�[�Z�b�g
        timer_ = time_;
    }
    timer_ -= delta_time;
}

// �`�� (�����\�����Ȃ�)
void EnemyGenerator::draw() const {}