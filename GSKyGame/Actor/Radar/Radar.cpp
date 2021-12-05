#include "Radar.h"
#include "../../World/IWorld.h"
#include "../Actor.h"
#include "../../AssetsID/Assets.h"

// ���[�_�[�̉�ʏ�̕\���ʒu
const GSvector2 RaderPosition{ 16.0f, 20.0f };
// ���[�_�[�̉摜�T�C�Y
const float RaderSize{ 256.0f };
// ���[�_�[�͈̔́i�v���[���[�ʒu����-20�`20�͈̔́j
const float RaderRange{ 20.0f };
// �O���E���h�̃T�C�Y
const float GroundSize{ 21.0f };

// �R���X�g���N�^
Radar::Radar(IWorld* world) {
    world_ = world;
    tag_ = "RadarTag";
    name_ = "Radar";
    enable_collider_ = false; // �Փ˔�������Ȃ�
}

void Radar::draw_gui() const{
    // ���[�_�[�̔w�i��`��
    static const GScolor4 back_color{ 1.0f, 1.0f, 1.0f, 0.5f }; // �������̎w��
    gsDrawSprite2D(Texture_Radar, &RaderPosition, NULL, NULL, &back_color, NULL, 0.0f);

    //�v���C���[�̌���
    Actor* player = world_->find_actor("Player");
    // ���[�_�[�̍��W�n�ɕϊ�����s��
    GSmatrix4 radar_matrix{ GS_MATRIX4_IDENTITY };
    if (player != nullptr) {
        // �v���[���[�̋t�s����擾�i�v���[���[�̈ʒu����Ƃ�����W���v�Z���邽�߁j
        radar_matrix = player->transform().worldToLocalMatrix();
    }

    // ���[�_�[�̏�������v���[���[�̐i�s�����ɍ��킹��
    radar_matrix *= GSmatrix4::rotateY(180.0f);

    for (float i = -GroundSize; i <= GroundSize; i += 1.0f){
        // �g�̓_�̐F�i���F�j
        static const GScolor frame_color{ 0.0f, 1.0f, 1.0f, 1.0 };
        // 4�ӂ̓_��`��
        draw_point(radar_matrix.multiplyPoint(GSvector3{ i, 0.0f, -GroundSize }), frame_color);
        draw_point(radar_matrix.multiplyPoint(GSvector3{ i, 0.0f,  GroundSize }), frame_color);
        draw_point(radar_matrix.multiplyPoint(GSvector3{ -GroundSize, 0.0f, i }), frame_color);
        draw_point(radar_matrix.multiplyPoint(GSvector3{ GroundSize, 0.0f, i }), frame_color);
    }

    // �G�̈ʒu��`��
    for (Actor* enemy : world_->find_actor_with_tag("EnemyTag")) {
        // �G�̓_�̐F�i�΁j
        static const GScolor enemy_color{ 0.0f, 1.0f, 0.0f, 1.0 };
        // �G�̓_��`��
        draw_point(radar_matrix.multiplyPoint(enemy->transform().position()), enemy_color);
    }
    // �v���[���[�̓_�̐F�i�ԁj
    static const GScolor player_color{ 1.0f, 0.0f, 0.0f, 1.0 };
    // �v���[���[�̈ʒu��`��i�v���[���[�̓_�͌��_�j
    draw_point(GSvector3{ 0.0f, 0.0f, 0.0f }, player_color);

}

void Radar::draw_point(const GSvector3& position, const GScolor& color) const{
    // ���[�_�[��2D���W�ɂ��Đ��K������(�\���͈͂�-1.0�`1.0�̍��W�ɕϊ�)
    GSvector2 normalized_position{ position.x / RaderRange,  position.z / RaderRange };

    // ���[�_�[�͈̔͊O�͕`�悵�Ȃ�
    if (normalized_position.x < -1.0f || normalized_position.x > 1.0f
        || normalized_position.y < -1.0f || normalized_position.y > 1.0f) {
        return;
    }

    // ���[�_�[�̉摜�T�C�Y�̔���(128�s�N�Z��)
    const float RaderHalfSize{ RaderSize / 2.0f };
    // ���[�_�[�̉摜�T�C�Y�ɍ��킹�Ċg�傷��(-128�`128)
    GSvector2 point_position = normalized_position * RaderHalfSize;
    // ���_�i�v���[���[�̈ʒu�j�����[�_�[�̒����ɂȂ�悤�ɕ␳(0�`256)
    point_position += GSvector2{ RaderHalfSize, RaderHalfSize };
    // ���[�_�[�̕\���ʒu�ɍ��킹�ĕ��s�ړ�
    point_position += RaderPosition;

    // �_�̉摜�̒��S�ʒu��␳�l����i�摜�̒��������_�ɂȂ�悤�Ɂj
    static const GSvector2 center{ 4.0f, 4.0f };
    // �_�̉摜��`��
    gsDrawSprite2D(Texture_RadarPoint, &point_position, NULL, &center, &color, NULL, 0.0f);


}


