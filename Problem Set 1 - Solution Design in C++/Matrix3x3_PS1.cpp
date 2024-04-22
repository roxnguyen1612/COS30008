#include"Matrix3x3.h"


// ----------------------- 2.1 matrix duplication -----------------------------
Matrix3x3 Matrix3x3::operator*(const Matrix3x3& aOther) const noexcept
{
    Vector3D lRow[3];

    for (int i = 0; i < 3; ++i)
    { 
        float lComponents[3] = { 0, 0, 0 }; 

        for (int j = 0; j < 3; ++j)
        { 
            lComponents[j] = this->row(i).dot(aOther.column(j));
        }
        lRow[i] = Vector3D(lComponents[0], lComponents[1], lComponents[2]);
    }

    return Matrix3x3(lRow[0], lRow[1], lRow[2]);
}

// --------------------------2.2 Determinate of a matrix -------------------------------
float Matrix3x3::det() const noexcept
{
    const Vector3D& lRow1 = this->row(0);
    const Vector3D& lRow2 = this->row(1);
    const Vector3D& lRow3 = this->row(2);

    float lDeterminant = 
          lRow1[0]* (lRow2[1] * lRow3[2] - lRow2[2] * lRow3[1])
        - lRow1[1] * (lRow2[0] * lRow3[2] - lRow2[2] * lRow3[0])
        + lRow1[2] * (lRow2[0] * lRow3[1] - lRow2[1] * lRow3[0]);

    return lDeterminant;
}

// ------------------------2.3 matrix transpose -------------------------
Matrix3x3 Matrix3x3::transpose() const noexcept 
{
    const Vector3D& lColumn1 = this->column(0);
    const Vector3D& lColumn2 = this->column(1);
    const Vector3D& lColumn3 = this->column(2);

    return Matrix3x3(lColumn1, lColumn2, lColumn3);
}

//------------------2.4 invertible matrix --------------
bool Matrix3x3::hasInverse() const noexcept
{
    if (this->det() != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ---------------2.5 matrix inverse----------------
Matrix3x3 Matrix3x3::inverse() const noexcept
{
    float lDeterminant = this->det();

    const Vector3D& lRow1 = this->row(0);
    const Vector3D& lRow2 = this->row(1);
    const Vector3D& lRow3 = this->row(2);

    Matrix3x3 lCofactorMatrix(
        Vector3D((lRow2[1] * lRow3[2] - lRow2[2] * lRow3[1]), -(lRow2[0] * lRow3[2] - lRow2[2] * lRow3[0]), (lRow2[0] * lRow3[1] - lRow2[1] * lRow3[0])),
        Vector3D(-(lRow1[1] * lRow3[2] - lRow1[2] * lRow3[1]), (lRow1[0] * lRow3[2] - lRow1[2] * lRow3[0]), -(lRow1[0] * lRow3[1] - lRow1[1] * lRow3[0])),
        Vector3D((lRow1[1] * lRow2[2] - lRow1[2] * lRow2[1]), -(lRow1[0] * lRow2[2] - lRow1[2] * lRow2[0]), (lRow1[0] * lRow2[1] - lRow1[1] * lRow2[0]))
    );

    Matrix3x3 lAdjugateMatrix = lCofactorMatrix.transpose();
    //incase not det
    if (lDeterminant == 0)
    {
        return Matrix3x3();
    }

    return lAdjugateMatrix * (1 / lDeterminant);
}

// ------------ 2.6 ostream for matrix --------------
std::ostream& operator<<(std::ostream& aOStream, const Matrix3x3& aMatrix)
{
    aOStream << "[";
    for (int i = 0; i < 3; ++i)
    {
        Vector3D lRow = aMatrix.row(i);
        aOStream << lRow.toString();
        if (i < 2)
        {
            aOStream  << ",";
        }
    }
    return aOStream << "]";
}