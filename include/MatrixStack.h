#ifndef _MATRIX_STACK_H_
#define _MATRIX_STACK_H_

#include <stack>
#include <cstring>
#include <gccore.h>

struct Mtx_s {
    Mtx data;
}__attribute__ ((packed));

class MatrixStack_t
{
public:
    MatrixStack_t() {Identity();}

    void Identity()
    {
        guMtxIdentity(current_matrix.data);
    }

    void setPosition(guVector &v)
    {
		current_matrix.data[0][3] = v.x;
		current_matrix.data[1][3] = v.y;
		current_matrix.data[2][3] = v.z;
	}

    void setPosition(float x, float y, float z)
    {
		current_matrix.data[0][3] = x;
		current_matrix.data[1][3] = y;
		current_matrix.data[2][3] = z;
	}

    void Translate(const guVector &v)
    {
		Mtx tmp;
        guMtxTrans(tmp, v.x, v.y, v.z);
        guMtxConcat(current_matrix.data, tmp, current_matrix.data);
    }

    void Translate(float x, float y, float z)
    {
		Mtx tmp;
        guMtxTrans(tmp, x, y, z);
        guMtxConcat(current_matrix.data, tmp, current_matrix.data);
    }

    void Rotate(guVector &v, float angle)
    {
        guMtxRotAxisRad(current_matrix.data, &v, angle);
    }

    void Rotate_X(float angle)
    {
		Mtx tmp;
        guMtxRotRad(tmp, 'x', angle);
        guMtxConcat(current_matrix.data, tmp, current_matrix.data);
    }

    void Rotate_Y(float angle)
    {
		Mtx tmp;
        guMtxRotRad(tmp, 'y', angle);
        guMtxConcat(current_matrix.data, tmp, current_matrix.data);
    }

    void Rotate_Z(float angle)
    {
		Mtx tmp;
        guMtxRotRad(tmp, 'z', angle);
        guMtxConcat(current_matrix.data, tmp, current_matrix.data);
    }

    void Scale(const guVector &v)
    {
        guMtxScale(current_matrix.data, v.x, v.y, v.z);
    }

    struct Mtx_s Top()
    {
        return current_matrix;
    }

    void baseMatrix(Mtx m)
    {
        memcpy(current_matrix.data, m, sizeof(Mtx));
    }

    void Push()
    {
        stack_matrix.push(current_matrix);
    }

    void Pop()
    {
        current_matrix = stack_matrix.top();
        stack_matrix.pop();
    }

    void Set(u32 pnidx)
    {
        GX_LoadPosMtxImm(current_matrix.data, pnidx);
    }

    MtxP const value_ptr()
    {
        return (MtxP)current_matrix.data;
    }

private:
    struct Mtx_s current_matrix;
    std::stack<struct Mtx_s> stack_matrix;
};

extern MatrixStack_t MatrixStack;

#endif
