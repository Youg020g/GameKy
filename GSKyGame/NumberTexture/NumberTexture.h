#ifndef NUMBER_TEXTURE_H_
#define NUMBER_TEXTURE_H_

#include <gslib.h>
#include <string>

// ナンバテクスチャ
class NumberTexture {
public:
    // コンストラクタ
    NumberTexture(GSuint texture, int width, int height);
    // 描画
    void draw(const GSvector2& position, int num, int digit, char fill = '0',
        const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) const;
    // 描画
    void draw(const GSvector2& position, int num,
        const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) const;
    // 描画
    void draw(const GSvector2& position, const std::string& num,
        const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f }) const;

    //得点のテキスト
    void score_text_texture(const GSvector2& position,
        const GScolor& color = GScolor{ 1.0f, 1.0f, 1.0f, 1.0f })const;

private:
    // フォント用のテクスチャ
    GSuint	texture_;
    // 文字の幅
    int	width_;
    // 文字の高さ
    int	height_;
};

#endif
