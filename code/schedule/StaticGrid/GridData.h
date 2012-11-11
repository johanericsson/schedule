#ifndef _GridData_H
#define _GridData_H

#include <vector>

class CGridData
{
public:
    CGridData(int iNumRows,int iNumColumns,
        const std::vector<std::vector<double> >& Contents);


    bool CheckValid() const;

    double


};



#endif
