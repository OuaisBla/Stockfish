#pragma once
#include <vector>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace mathalgo
{
	using namespace std;

	template<class T>
	class matrix
	{
	public:
		matrix(size_t nRows, size_t nCols) : 
			m_nRows( nRows ), 
			m_nCols( nCols ), 
			m_oData( nRows*nCols, 0 )
		{
			if ( !nRows || !nCols )
			{
				throw range_error( "invalid matrix size" );
			}
		}

		static matrix identity( size_t nSize )
		{
			matrix oResult( nSize, nSize );

			size_t nCount = 0;
			std::generate( oResult.m_oData.begin(), oResult.m_oData.end(), 
				[&nCount, nSize]() { return !(nCount++%(nSize + 1)); } );

			return oResult;
		}

		inline T& operator()(size_t nRow, size_t nCol)
		{
			if ( nRow >= m_nRows || nCol >= m_nCols )
			{
				throw out_of_range( "position out of range" );
			}

			return m_oData[nCol+m_nCols*nRow];
		}

		inline matrix operator*(matrix& other)
		{
			if ( m_nCols != other.m_nRows )
			{
				throw domain_error( "matrix dimensions are not multiplicable" );
			}

			matrix oResult( m_nRows, other.m_nCols );
			for ( size_t r = 0; r < m_nRows; ++r )
			{
				for ( size_t ocol = 0; ocol < other.m_nCols; ++ocol )
				{
					for ( size_t c = 0; c < m_nCols; ++c )
					{
						oResult(r,ocol) += (*this)(r,c) * other(c,ocol);
					}
				}
			}

			return oResult;
		}

		inline matrix transpose()
		{
			matrix oResult( m_nCols, m_nRows );
			for ( size_t r = 0; r < m_nRows; ++r )
			{
				for ( size_t c = 0; c < m_nCols; ++c )
				{
					oResult(c,r) += (*this)(r,c);
				}
			}
			return oResult;
		}

		inline size_t rows() 
		{
			return m_nRows;
		}

		inline size_t cols() 
		{
			return m_nCols;
		}

		inline vector<T> data()
		{
			return m_oData;
		}

		void print()
		{
			for ( size_t r = 0; r < m_nRows; r++ )
			{
				for ( size_t c = 0; c < m_nCols; c++ )
				{
					std::cout << (*this)(r,c) << "\t";
				}
				std::cout << std::endl;
			}
		}

	private:
		std::vector<T> m_oData;

		size_t m_nRows;
		size_t m_nCols;
	};
};