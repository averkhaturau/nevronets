#ifndef _LX_NNLAYER_H_
#define _LX_NNLAYER_H_

#include "gsettings.h"
#include "activafunc.h"
#include <assert.h>
#include <memory.h>

/*
	Слой - объенинение входов и весов нейронов, которые могут обрабатываться одновременно,
	т.е. в случае сети с обратной связью данные, идущие на предыдущий слой просто копируются
	во вход этого слоя.
	Нейрон как таковой выполняет вычисление скалярного произведения
	вектора своих весов и вектора всех входов слоя и активационной функции.
			acson = f_act(dotproduct(rates, inputs)

	Эти действия над нейоном означают следующие действия над слоем:
			acson1 = f_act(dotproduct(rates1, inputs)
			...
			acsonN = f_act(dotproduct(ratesN, inputs)

	или
			acson_i = f_act(inputs_j^T * rates_ji)

	Итак, у нас есть матрица весов rates_ji (по строкам - нейроны, по столбцам - вес для соотв. входа)
	и есть вектор входов, один для слоя.
*/

/**
	Слой есть матрица весов (нейроны х веса) и вектор входных данных.
	Число строк матрицы = размерности вектора вх.данных = числу нейронов на предыдущем слое.
	Число столбцов матрицы = числу нейросов на текущем слое.

	В случае персепрона данные движутся от слоя к слою, если это сеть и feedback,
	то результат на аксонах кроме того, что идёт на следующий слой, ещё и копируется
	во входной вектор (дендриды) этого слоя. Таким образом, объект "слой"
	для всех топологий нс одинаков.

	В случае параллельной реализации в каджом процессе будет свой слой,
	но у него будет только часть нейронов => только часть строк матрицы весов,
	и результатом будет только часть вектора выходов, в то время как входы
	будут одинаковы во всех процессах. Сбор всего слоя данных можно выполнять
	с помощью MPI_Allgatherv. Таким образом, объект "слой" одинаков для
	параллельной и последовательной реализации.
*/
class CNNLayer
{
	friend class CNNBackPropagation;
protected:
	/// Матрица
	SYGDATATYPE *m_pRates;
	/// Вектор входов
	SYGDATATYPE *m_pInput;
	/// Смещения входов
	SYGDATATYPE *m_pDispl;

	unsigned int m_iThisLayerSize;
	unsigned int m_iPrevLayerSize;

	CActi m_funcacti;

public:
	// конструктор по умолчанию - требует последующего вызова CNNLayer::Construct
	CNNLayer();
	/** Создание слоя нейронов
	@param iThisLayerSize Число нейронов на этом слое (равно числу выходов (аксонов))
	@param iPrevLayerSize Число нейронов на предыдущем слое (равно числу входов (дендридов))
	*/
	explicit CNNLayer(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, const CActi &a_funcacti);
	/// Деструктор
	~CNNLayer();

	/** Устанавливает значения для матрицы весов
	@param a_pRates значения элементов матрицы весов
	@param size количество элементов (для проверки)
	*/
	void SetRates(const SYGDATATYPE *a_pRates, const unsigned int size);

	/** Устанавливает значения для вектора входов. Используется при выполнении прогона данных по нс.
	@param a_pInput значения элементов вектора входов
	@param size количество элементов (для проверки)
	*/
	void SetInput(const SYGDATATYPE *a_pInput, const unsigned int size);

	/** Устанавливает значения для вектора смещений.
	@param a_pDispl значения элементов вектора смещений
	@param size количество элементов (для проверки)
	*/
	void SetDispl(const SYGDATATYPE *a_pDispl, const unsigned int size);

	/** Возвращает значения для матрицы весов
	@param size возвращает количество элементов (для проверки)
	@return pointer to matrix
	*/
	SYGDATATYPE *GetRates(unsigned int &size);

	/** Возвращает значения для матрицы смещений
	@param size возвращает количество элементов (для проверки)
	@return pointer to matrix
	*/
	SYGDATATYPE *GetDispl(unsigned int &size);


	/** Возвращает значения для вектора входов. Используется при выполнении прогона данных по нс.
	@param size возвраащет количество элементов (для проверки)
	@return pointer to vector
	*/
	SYGDATATYPE *GetInput(unsigned int &size);


	/** Устанавливает значения для вектора входов. Используется при выполнении прогона данных по нс.
	@param a_pOutput возвращает значения элементов вектора выходов (аксонов)
	@param size количество элементов (для проверки)
	*/
	void GetAscons(SYGDATATYPE *a_pOutput, const unsigned int size);

	inline CActi GetActiFunc()const{return m_funcacti;};

	inline unsigned int GetThisLayerSize()const{return m_iThisLayerSize;};
	inline unsigned int GetPrevLayerSize()const{return m_iPrevLayerSize;};

protected:
	void Destruct();
public:
	void Construct(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, const CActi &a_funcacti);
};

/**
	Разновидность класса слоя нейоов, который хранит входы и суммы нейронов,
	 что позволяет опимизировать обучение нс.
*/
class CNNLayer_store : public CNNLayer
{
protected:
	SYGDATATYPE *m_pSS; // аргументы акивационой функции

public:
	CNNLayer_store(){m_pSS=0;};
	void Construct(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, CActi &a_funcacti)
	{
		CNNLayer::Construct(iThisLayerSize, iPrevLayerSize, a_funcacti);
		
		m_pInput = new SYGDATATYPE[m_iPrevLayerSize];
		m_pSS    = new SYGDATATYPE[m_iThisLayerSize];
	};

	~CNNLayer_store()
	{
		if (m_pSS)
			delete[] m_pSS;
		if (m_pInput)
			delete[] m_pInput;
	};

	void SetInput(const SYGDATATYPE *a_pInput, const unsigned int size)
	{
		assert(size == m_iPrevLayerSize);
		memcpy(m_pInput, a_pInput, size*sizeof(SYGDATATYPE));
		//m_pInput = const_cast<SYGDATATYPE*>(a_pInput);
	};

	void GetAscons(SYGDATATYPE *a_pOutput, const unsigned int size)
	{
		assert(size == m_iThisLayerSize);
		assert(m_pInput != 0);

		unsigned int i,ii;
		for (i=ii=0; i<m_iThisLayerSize; ++i,ii+=m_iPrevLayerSize)
		{
			SYGDATATYPE tempVal=0;
			for (unsigned int j=0;j<m_iPrevLayerSize;++j)
				tempVal+=m_pInput[j]*m_pRates[j+ii];
			m_pSS[i] = tempVal; // вот это изменилось
			a_pOutput[i] = (*m_funcacti.f)(tempVal);
		}
	};

	SYGDATATYPE *GetS()const{return m_pSS;};
};


#endif //_LX_NNLAYER_H_
