#ifndef _LX_NN_TRAINING_H___
#define _LX_NN_TRAINING_H___

//#define _LX_IS_LOG_ERROR_IN_ITERATIONS

#include <vector>
#include <math.h>

#include "nn1x.h"
#include "nnForwardPropagation.h"
#include "nnifc.h"

/*
	��� ����������� �����, ��������� ����������� ��������� ����.
	��� ������� ��������� �������� � ����������� ������.
*/


class CNNTraining
{
protected:

public:
	/** ����������� ������� ����
	@param a_pInput ������ ������� ������
	@param a_pExpOut ������ ��������� �������� ������
	@param i_size ������ ������� ������ - ������ ��� �������� ������������
	@param o_size ������ �������� ������ � ������� ������ - ������ ��� �������� ������������
	@param a_NN - ���������� ��������� ���� (���������)
	@param a_pErrorVect ������ ������
	*/
	/*void GetError(
		const SYGDATATYPE *a_pInput, const unsigned int i_size,
		const SYGDATATYPE *a_pExpOut, const unsigned int o_size,
		CNeuroNet *a_NN, SYGDATATYPE *a_pErrorVect)
	{
		a_NN->Propagate(a_pInput, i_size, a_pErrorVect, o_size);
		for (unsigned int i=0; i<o_size; ++i)
		{
			a_pErrorVect[i] -= a_pExpOut[i];
		}
	};*/
	static SYGDATATYPE GetDiffNorm(const SYGDATATYPE* a_vec1, const SYGDATATYPE* a_vec2, const unsigned int a_uiSize)
	{
		SYGDATATYPE Result = 0;
		for (unsigned int ui=0; ui<a_uiSize; ++ui)
		{
			SYGDATATYPE a = a_vec1[ui]-a_vec2[ui];
			Result += a*a;
		}
		return Result;
	};
};

/// ��������� ������ ���� - ������������������ ������ � �������
class CTrainingInstance
{
protected:
	SYGDATATYPE *m_pInput;
	unsigned int m_i_size;
	SYGDATATYPE *m_pExpOut;
	unsigned int m_o_size;

public:
	CTrainingInstance(const SYGDATATYPE *a_pInput, const unsigned int i_size,
					  const SYGDATATYPE *a_pExpOut, const unsigned int o_size)
	{
		m_i_size  = i_size;
		m_o_size  = o_size;
		m_pInput  = new SYGDATATYPE[m_i_size];
		m_pExpOut = new SYGDATATYPE[m_o_size];
		memcpy(m_pInput,  a_pInput,  m_i_size*sizeof(SYGDATATYPE));
		memcpy(m_pExpOut, a_pExpOut, m_o_size*sizeof(SYGDATATYPE));
	};

	CTrainingInstance &operator =(const CTrainingInstance &a_TI)
	{
		if (a_TI.m_i_size != this->m_i_size)
		{
			delete[] m_pInput;
			m_pInput = new SYGDATATYPE[m_i_size];
			this->m_i_size  = a_TI.m_i_size;
		}
		if (a_TI.m_o_size != this->m_o_size)
		{
			delete[] m_pExpOut;
			m_pExpOut = new SYGDATATYPE[m_o_size];
			this->m_o_size  = a_TI.m_o_size;
		}

		memcpy(m_pInput,  a_TI.GetInput(),  m_i_size*sizeof(SYGDATATYPE));
		memcpy(m_pExpOut, a_TI.GetOutput(), m_o_size*sizeof(SYGDATATYPE));
		return *this;
	};

	CTrainingInstance(const CTrainingInstance &a_TI)
	{
		m_i_size  = a_TI.m_i_size;
		m_o_size  = a_TI.m_o_size;
		m_pInput  = new SYGDATATYPE[m_i_size];
		m_pExpOut = new SYGDATATYPE[m_o_size];
		memcpy(m_pInput,  a_TI.GetInput(),  m_i_size*sizeof(SYGDATATYPE));
		memcpy(m_pExpOut, a_TI.GetOutput(), m_o_size*sizeof(SYGDATATYPE));
	};


	~CTrainingInstance()
	{
		delete[] m_pInput;
		delete[] m_pExpOut;
	};

	SYGDATATYPE *GetInput() const{return m_pInput;};
	unsigned int GetISize() const{return m_i_size;};
	SYGDATATYPE *GetOutput()const{return m_pExpOut;};
	unsigned int GetOSize() const{return m_o_size;};
};

class CTrainingInstances : protected std::vector<CTrainingInstance>
{
	friend class CNNIfc;
public:
	CTrainingInstances(char *filename, CNNIfc &a_Ifc)
	{
		a_Ifc.ReadInstanses(filename, this);
	};

	CTrainingInstances(){};

	std::vector<CTrainingInstance>::size_type size() const
	{return ((std::vector<CTrainingInstance>*)this)->size();};

	std::vector<CTrainingInstance>::const_reference operator[](size_type _P) const
	{return ((std::vector<CTrainingInstance>*)this)->operator [] (_P); };

	std::vector<CTrainingInstance>::reference operator[](size_type _P)
	{return ((std::vector<CTrainingInstance>*)this)->operator [] (_P); };

	void AddInstance(CTrainingInstance a_Instance)
	{
		this->push_back(a_Instance);
	};

	void RemoveInstance(unsigned int Index)
	{
		assert(Index<this->size());
		erase(begin()+Index);
	};

	CTrainingInstances & operator = (const CTrainingInstances & a_TIs)
	{
		(std::vector<CTrainingInstance>(*this)).operator =((std::vector<CTrainingInstance>)a_TIs);
	};

	SYGDATATYPE CalcLipshic()
	{// ��������� ������� ���� �� ���������� inf_i_j(||fi-fj||/||xi-xj||)
		SYGDATATYPE Result = 0;
		const unsigned int fsize = begin()->GetOSize();
        const unsigned int xsize = begin()->GetISize();
		for (iterator Iter1=begin();Iter1!=end();++Iter1)
		for (iterator Iter2=begin();Iter2!=end();++Iter2)
		{	//���������� ��� �������
			if (Iter1==Iter2)
				continue;
			SYGDATATYPE CurLip =
				GetDiffNorm(Iter1->GetOutput(), Iter2->GetOutput(), fsize) /
				GetDiffNorm(Iter1->GetInput(), Iter2->GetInput(), xsize);
			if (Result < CurLip)
				Result = CurLip;
		}
		return Result;
	};

	SYGDATATYPE CalcLipshicFor(unsigned int i,unsigned int j)
	{	//made by zinevich
		///WARNING!!!: can work incorrect
		// ���������� ��������� ������� ��� ���� �������� � �������� i � j)
		SYGDATATYPE Result = 0;
		const unsigned int fsize = begin()->GetOSize();
		const unsigned int xsize = begin()->GetISize();
		iterator Iter1=begin()+i, Iter2=begin()+j;
				SYGDATATYPE CurLip =
				GetDiffNorm(Iter1->GetOutput(), Iter2->GetOutput(), fsize) /
				GetDiffNorm(Iter1->GetInput(), Iter2->GetInput(), xsize);
				Result = CurLip;
				return Result;
	};


protected:
	SYGDATATYPE GetDiffNorm(const SYGDATATYPE* a_vec1, const SYGDATATYPE* a_vec2, const unsigned int a_uiSize)
	{
		return sqrt( CNNTraining::GetDiffNorm(a_vec1, a_vec2, a_uiSize) );
	};
};

class CNNBackPropagation : public CNNTraining
{

public:
	/** �������� ������� ��������� ���������������� ������
	@param CNeuroNet *a_pNN -- ��������� �� ��������� ��������� ����
	@param const SYGDATATYPE **a_pInputs -- ������ �������� ������ ��������� ��������
	@param const unsigned int i_num -- ���������� ��������� ��������
	@param const unsigned int i_size
	@param const SYGDATATYPE **a_pOutputs -- ������ �������� ������� ��������� ��������
	@param const unsigned int o_num
	@param const unsigned int o_size
	@param double a_Error -- ��������� �����������
	@param unsigned int a_maxiter -- ������� �������� �� ������, ��� ���� ��������. ���� 0, ������������.
	@param unsigned int iter_start -- � ������ ������� �������� ��������
	@param const SYGDATATYPE metta -- ����������� ������� ����������
	*/
	static void Train(CNNForwardPropagation<CNNLayer> *a_pNN, CTrainingInstances *a_pTI,
		double a_Error, unsigned int a_maxiter=0, unsigned int iter_start=0, SYGDATATYPE metta=-0.1377)
	{//CNNIfc aaaa;
#define MY_MAX_VAL 100
		const unsigned int nI = a_pNN->GetINum();
		const unsigned int nO = a_pNN->GetONum();
        const unsigned int _NI = a_pTI->size();
		const unsigned int uiTIsize = a_pTI->size();
		assert(nI && nO && _NI && uiTIsize);
        SYGDATATYPE Error=a_Error*2; // ��������� �������� - ���, ����� ��������

		//const SYGDATATYPE metta=-0.1377; // ����������� �������� ��������

		// ������ ����� ����
		unsigned int iBufSize;
		SYGDATATYPE *pNetBuf = a_pNN->GetBuf(iBufSize);

		// �������� �� � ����������� ������������� �������� ������� � ������������ ����
		// ��� ���������� ���������� ������, � ����� ��� ��������� �����������
		CNNForwardPropagation<CNNLayer_store> NNs(*a_pNN);

		SYGDATATYPE *pDelta = //new SYGDATATYPE[iBufSize*2];
			(SYGDATATYPE*)malloc( sizeof(SYGDATATYPE) * iBufSize * 2 );

		bool bIsNumItersLimited = (a_maxiter != 0);
		for (unsigned int j=0;Error>a_Error;++j)
        {
			bool IsOwerflowSimilarErrorOccured = false;
			unsigned int j_ = (j+iter_start)%uiTIsize ;
			//aaaa.Save2file("debug-lastlayer.nn", a_pNN);

#ifdef _LX_IS_LOG_ERROR_IN_ITERATIONS
			{ // �������� ������
				FILE *faaa=fopen("error-backbprop.log","a");
				Error = 0;
				for (unsigned int ii =0; ii<uiTIsize; ++ii)
				{
					NNs.Propagate(
						(*a_pTI)[ii].GetInput(), (*a_pTI)[ii].GetISize(),
						pNetBuf,                 (*a_pTI)[ii].GetOSize() );

					double Error1 = GetDiffNorm(pNetBuf, (*a_pTI)[ii].GetOutput(), (*a_pTI)[ii].GetOSize() );
					//Error += Error1;
					fprintf(faaa, "%2.1e\t",Error);
				} fprintf(faaa, "j_=%d",j_);
				fprintf(faaa, "\n");
				fclose(faaa);
			}
#endif //_LX_IS_LOG_ERROR_IN_ITERATIONS

#ifndef _LX_IS_LOG_ERROR_IN_ITERATIONS
			{ 	Error = 0;
				for (unsigned int ii =0; ii<uiTIsize; ++ii)
				{
					NNs.Propagate(
						(*a_pTI)[ii].GetInput(), (*a_pTI)[ii].GetISize(),
						pNetBuf,                 (*a_pTI)[ii].GetOSize() );

					Error += GetDiffNorm(pNetBuf, (*a_pTI)[ii].GetOutput(), (*a_pTI)[ii].GetOSize() );
				}
			}
#endif //_LX_IS_LOG_ERROR_IN_ITERATIONS

			//��� �������� ��������� ����
			NNs.Propagate(
				(*a_pTI)[j_].GetInput(),  (*a_pTI)[j_].GetISize(),
				pNetBuf,                  (*a_pTI)[j_].GetOSize() );

			//Error = GetDiffNorm(pNetBuf, (*a_pTI)[j_].GetOutput(), (*a_pTI)[j_].GetOSize() );

			// ���������� Y, s, w, �������� ������ � ���������� �����
			// ���������� ����� �������� � ����� a_pNN (� ����� �������� � ��� ���� �� NNs)

			{	//��� ���������� ����
				unsigned int ui1,ui2, nnn;
				for (ui1=0;ui1<(*a_pTI)[j_].GetOSize();++ui1)
				{
					double y_asterisk_minus_y = ( pNetBuf[ui1] - (*a_pTI)[j_].GetOutput()[ui1] );
					//if (fabs(y_asterisk_minus_y) > MY_MAX_VAL)
					//	pDelta[ui1+iBufSize*((NNs.m_numlayers-1)%2)] = 0.5-double(rand()%MY_MAX_VAL)/MY_MAX_VAL;
					//else
					{
					double dy_div_ds = NNs.m_layers[NNs.m_numlayers-1].GetActiFunc().dfds(
							NNs.m_layers[NNs.m_numlayers-1].GetS()[ui1]   );
				//	if (fabs(dy_div_ds) > MY_MAX_VAL || fabs(dy_div_ds) == 0)
				//		dy_div_ds = 0.5-double(rand()%MY_MAX_VAL)/MY_MAX_VAL;

					pDelta[ui1+iBufSize*((NNs.m_numlayers-1)%2)] = y_asterisk_minus_y * dy_div_ds;
					}
				}
				for (ui1=0;ui1<NNs.m_layers[NNs.m_numlayers-1].m_iThisLayerSize;++ui1)
				for (ui2=0;ui2<NNs.m_layers[NNs.m_numlayers-1].m_iPrevLayerSize;++ui2)
				{
					double delta_w_i_j = metta *
						pDelta[ui1+iBufSize*((NNs.m_numlayers-1)%2)] *
						NNs.m_layers[a_pNN->m_numlayers-1].GetInput(nnn)[ui2];

				//	if (fabs(delta_w_i_j) >= 0.9)
				//		delta_w_i_j = 0.5-double(rand()%MY_MAX_VAL)/MY_MAX_VAL;

					a_pNN->m_layers[a_pNN->m_numlayers-1].
						GetRates(nnn)[ui1 * NNs.m_layers[NNs.m_numlayers-1].
						m_iPrevLayerSize +ui2] = delta_w_i_j;
				}
			}

			{	// ��������� ��������� ����
				unsigned int ui1,ui2, nnn;
			for (signed int uiNumLayer=a_pNN->m_numlayers-2; uiNumLayer>=0; --uiNumLayer)
			{
				for (ui1=0;ui1<NNs.m_layers[uiNumLayer].m_iPrevLayerSize;++ui1)
				{// ������ ������ ������
					double ssss = 0;
					for (unsigned int iiii=0; iiii<NNs.m_layers[uiNumLayer+1].m_iThisLayerSize; ++iiii)
					{
						unsigned int uiIndex1 = ui1 * NNs.m_layers[uiNumLayer+1].m_iPrevLayerSize+iiii;
						unsigned int uiIndex2 = iBufSize*((uiNumLayer+1)%2) + iiii;
						ssss += a_pNN->m_layers[uiNumLayer+1].
							GetRates(nnn)[uiIndex1] * pDelta[uiIndex2];
					}
					pDelta[ui1+iBufSize*((uiNumLayer+1)%2)] = ssss *
						NNs.m_layers[uiNumLayer].GetActiFunc().dfds(
							NNs.m_layers[uiNumLayer].GetS()[ui1]   );
				}
				for (ui1=0;ui1<NNs.m_layers[uiNumLayer].m_iThisLayerSize;++ui1)
				for (ui2=0;ui2<NNs.m_layers[uiNumLayer].m_iPrevLayerSize;++ui2)
				{
					unsigned int Index1 = ui1 * NNs.m_layers[uiNumLayer].m_iPrevLayerSize;
					double delta_mult_y = pDelta[ui1+iBufSize*((uiNumLayer+1)%2)]*
							NNs.m_layers[uiNumLayer].GetInput(nnn)[ui2];

			//		if (fabs(delta_mult_y) > 0.9 //||
						//fabs(delta_mult_y) == numeric_limits<double>::infinity() ||
						//fabs(delta_mult_y) == numeric_limits<double>::quiet_NaN()
			//			)
			//			delta_mult_y = 0.5-double(rand()%MY_MAX_VAL)/MY_MAX_VAL;

					a_pNN->m_layers[uiNumLayer].GetRates(nnn)[Index1 + ui2] =
							metta * delta_mult_y;
				}
			}
			}


			// � a_pNN � ��� ���������� �����, � NNs - ������ ����, ����� � �����
			// ����� ��������� �������� � NNs
			for (unsigned int uiLas=0; uiLas<NNs.m_numlayers; ++uiLas)
			{ // �� �����
				unsigned int nnn, iooooo;
				SYGDATATYPE *pRates = NNs.m_layers[uiLas].GetRates(nnn);
				for (unsigned int uiI = 0; uiI<nnn; ++uiI)
				{
					if (fabs(pRates[uiI]) >= MY_MAX_VAL)
					{
						IsOwerflowSimilarErrorOccured = true;
						pRates[uiI] = 0.5-double(rand()%MY_MAX_VAL)/MY_MAX_VAL;
					//	a_pNN->m_layers[uiLas].GetRates(iooooo)[uiI] = double(rand()%MY_MAX_VAL)/MY_MAX_VAL;
					}
					else
						pRates[uiI] += a_pNN->m_layers[uiLas].GetRates(iooooo)[uiI];
				}
			}

			if (IsOwerflowSimilarErrorOccured)
			{
				std::cerr<< "IsOwerflowSimilarErrorOccured is true :-(\n";
			}

			if (bIsNumItersLimited)
				if (--a_maxiter == 0)
					break;
        }
		//delete[]
	//	free(pDelta);

		// � ������ ��������� �������� � a_pNN
		for (unsigned int i=0; i<NNs.m_numlayers; ++i)
		{ // �� �����
			unsigned int ui;
			SYGDATATYPE *p=0;
			p=NNs.m_layers[i].GetRates(ui);
			a_pNN->m_layers[i].SetRates(p, ui);

			p=NNs.m_layers[i].GetDispl(ui);
			a_pNN->m_layers[i].SetDispl(p, ui);
		}

	};

};

class CNNStochastic : public CNNTraining
{

public:

	// @param unsigned int a_maxiter ��������� �� ����� ��� ������� �������� ������, ���� 0, �� ������������ ��������
	static void Train(CNNForwardPropagation<CNNLayer> *a_pNN, CTrainingInstances *a_pTI,
		double a_Error, unsigned int a_maxiter=0, SYGDATATYPE SkorostSkhodimosti=0.1)
	{
		const unsigned int nI = a_pNN->GetINum();
		const unsigned int nO = a_pNN->GetONum();
		const unsigned int _NI = a_pTI->size();
		const unsigned int uiTIsize = a_pTI->size();
		assert(nI && nO && _NI && uiTIsize);
        SYGDATATYPE Error[2];

		// ������ ����� ����
		unsigned int iBufSize;
		SYGDATATYPE *pNetBuf = a_pNN->GetBuf(iBufSize);


		//�������, ��� ������������ ����� �������� ��� ���������
		CNNForwardPropagation<CNNLayer> MyNN(*a_pNN);
		CNNForwardPropagation<CNNLayer> *pMyNNpoiters[2];
		short int best_index = 1, worst_index = (best_index^1)&1;
		pMyNNpoiters[0] = &MyNN; pMyNNpoiters[1] = a_pNN;

		{//������ ������ ��� ���� ��������
			Error[best_index]=0;
			for (unsigned int j_=0; j_<_NI; ++j_)
			{
				pMyNNpoiters[best_index]->Propagate(
					(*a_pTI)[j_].GetInput(),  (*a_pTI)[j_].GetISize(),
					pNetBuf,                  (*a_pTI)[j_].GetOSize() );

				Error[best_index] +=
					GetDiffNorm(pNetBuf, (*a_pTI)[j_].GetOutput(), (*a_pTI)[j_].GetOSize() );
			}
		}

		bool bIsNumItersLimited = (a_maxiter != 0);
		unsigned int _numiters=0;
		signed int numDullIters=0;
		while (Error[best_index] > a_Error)
		{
			{// ������������ ���� � ����� �� ���������
				for (unsigned int uiLas=0; uiLas<pMyNNpoiters[worst_index]->m_numlayers; ++uiLas)
				{ // �� �����
					unsigned int nnn;
					SYGDATATYPE *pRates_w = pMyNNpoiters[worst_index]->m_layers[uiLas].GetRates(nnn);
					SYGDATATYPE *pRates_b = pMyNNpoiters[best_index]->m_layers[uiLas].GetRates(nnn);
					for (unsigned int uiI = 0; uiI<nnn; ++uiI)
					{// �������� � ��������� �������� ����
						pRates_w[uiI] = pRates_b[uiI] + SkorostSkhodimosti*(5e2-rand()%1000)*Error[best_index]/(_NI*500);
					}
				}
			}

			{//������ ������ ��� ���� �������� ������ �����������
				Error[worst_index]=0;
				for (unsigned int j_=0; j_<_NI; ++j_)
				{
					pMyNNpoiters[worst_index]->Propagate(
						(*a_pTI)[j_].GetInput(),  (*a_pTI)[j_].GetISize(),
						pNetBuf,                  (*a_pTI)[j_].GetOSize() );

					Error[worst_index] += GetDiffNorm(pNetBuf, (*a_pTI)[j_].GetOutput(), (*a_pTI)[j_].GetOSize() );
				}
			}

	#ifdef _LX_IS_LOG_ERROR_IN_ITERATIONS
				{ // �������� ������
					FILE *faaa=fopen("error-stoch.log","a");
					/*for (unsigned int ii =0; ii<uiTIsize; ++ii)
					{
						pMyNNpoiters[worst_index]->Propagate(
							(*a_pTI)[ii].GetInput(), (*a_pTI)[ii].GetISize(),
							pNetBuf,                 (*a_pTI)[ii].GetOSize() );

						double Error = GetDiffNorm(pNetBuf, (*a_pTI)[ii].GetOutput(), (*a_pTI)[ii].GetOSize() );

					*/	fprintf(faaa, "%le\t%le\titer=%d\n", Error[best_index], Error[worst_index], _numiters);
					/*}
					fprintf(faaa, "\n");
					*/
					fclose(faaa);
				}
	#endif //_LX_IS_LOG_ERROR_IN_ITERATIONS

			if (Error[worst_index] < Error[best_index])
			{
				best_index = worst_index;
				worst_index = best_index ^1;
				numDullIters = 0;
			}
			else
			{
				++numDullIters;
			}
			++_numiters;
			if (numDullIters > 50)
			{
				SkorostSkhodimosti *= .888;
				numDullIters = 0;
			}
			if (bIsNumItersLimited)
				if (--a_maxiter == 0)
					break;

		}// end of while

		// � ������ ��������� �������� � a_pNN
		if (best_index != 1)
		for (unsigned int i=0; i<pMyNNpoiters[best_index]->m_numlayers; ++i)
		{ // �� �����
			unsigned int ui;
			SYGDATATYPE *p=0;
			p=pMyNNpoiters[best_index]->m_layers[i].GetRates(ui);
			a_pNN->m_layers[i].SetRates(p, ui);

			p=pMyNNpoiters[best_index]->m_layers[i].GetDispl(ui);
			a_pNN->m_layers[i].SetDispl(p, ui);
		}

	};
};

#endif //_LX_NN_TRAINING_H___
