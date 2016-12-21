#include "gtest/gtest.h"
#include "tensor/dense_tensor.h"
#include "tensor/t_data.h"
#include <type_traits>

using namespace gnn;

#define sqr(x) ((x) * (x))

TEST(TensorTest, ReshapeSize)
{
	Tensor* t = new DTensor<CPU, float>();
	t->Reshape({2, 3, 4});

	auto& mat = t->Derived<CPU, DENSE, float>();

	ASSERT_EQ(2 * 3 * 4, mat.data->mem_size);
}

TEST(TensorTest, BroadcastMulCol)
{
	DTensor<CPU, float> x, y;
	x.Reshape({5, 3}); 
	y.Reshape({5, 1});
	x.Fill(1.0);
	for (int i = 0; i < 5; ++i)
	{
		x.data->ptr[i * 3 + 1] = 2.0;
		x.data->ptr[i * 3 + 2] = 3.0;
	}
	for (int i = 0; i < 5; ++i)
		y.data->ptr[i] = i + 1;

	x.ElewiseMul(y);
	for (int i = 0; i < (int)x.rows(); ++i)
		for (int j = 0; j < (int)x.cols(); ++j)
			ASSERT_EQ(x.data->ptr[i * x.cols() + j], (i + 1) * (j + 1));
}

TEST(TensorTest, BroadcastMulRow)
{
	DTensor<CPU, float> x, y;
	x.Reshape({3, 5}); 
	y.Reshape({1, 5});
	x.Fill(1.0);
	for (int i = 0; i < 5; ++i)
	{
		x.data->ptr[5 + i] = 2.0;
		x.data->ptr[10 + i] = 3.0;
	}

	for (int i = 0; i < 5; ++i)
		y.data->ptr[i] = i + 1;
	x.ElewiseMul(y);
	for (int i = 0; i < (int)x.rows(); ++i)
		for (int j = 0; j < (int)x.cols(); ++j)
			ASSERT_EQ(x.data->ptr[i * x.cols() + j], (i + 1) * (j + 1));
}

TEST(TensorTest, RandNorm)
{
	Tensor* t = new DTensor<CPU, double>();
	t->Reshape({100, 500, 100});	
	auto& mat = t->Derived<CPU, DENSE, double>();

	mat.SetRandN(5.0, 0.1);

	double s = 0.0;
	for (size_t i = 0; i < mat.data->mem_size; ++i)
		s += mat.data->ptr[i];
	s /= mat.shape.Count();
	double err = fabs(s - 5.0);
	EXPECT_LE(err, 1e-4);

	double ss = 0.0;
	for (size_t i = 0; i < mat.data->mem_size; ++i)
		ss += sqr(mat.data->ptr[i] - s);
	ss = sqrt(ss / mat.shape.Count());
	err = fabs(ss - 0.1);
	EXPECT_LE(err, 1e-4);
}

TEST(TensorTest, Zero)
{
	Tensor* t = new DTensor<CPU, float>();
	t->Reshape({2, 3, 4});
	auto& mat = t->Derived<CPU, DENSE, float>();
	mat.Zeros();

	int ans = mat.ASum();
	ASSERT_EQ(0, ans);
}

TEST(TensorTest, AsScalar)
{
	Tensor* t = new DTensor<CPU, float>();
	t->Reshape({1, 1, 1});
	auto& mat = t->Derived<CPU, DENSE, float>();
	mat.Zeros();

	ASSERT_EQ(0, mat.AsScalar());
}

TEST(TensorTest, Fill)
{
	Tensor* t = new DTensor<CPU, int>();
	t->Reshape({2, 3, 4});
	auto& mat = t->Derived<CPU, DENSE, int>();
	mat.Fill(2);

	float ans = 0;
	for (size_t i = 0; i < mat.data->mem_size; ++i)
		ans += mat.data->ptr[i];

	ASSERT_EQ(48, ans);
}

TEST(TensorTest, Compile)
{
	EXPECT_EQ(1, 1);
}