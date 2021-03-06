
/*
Copyright (c) 2013 Maximus5
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

class CRefRelease
{
private:
	LONG volatile mn_RefCount;
	static const LONG REF_FINALIZE = 0x7FFFFFFF;

protected:
	virtual void FinalRelease() = 0;

public:
	CRefRelease()
	{
		mn_RefCount = 1;
	};
	
	void AddRef()
	{
		if (!this)
		{
			_ASSERTE(this!=NULL);
			return;
		}
	
		InterlockedIncrement(&mn_RefCount);
	};

	int Release()
	{
		if (!this)
			return 0;

		InterlockedDecrement(&mn_RefCount);
	
		_ASSERTE(mn_RefCount>=0);
		if (mn_RefCount <= 0)
		{
			mn_RefCount = REF_FINALIZE; // �������������, ����� �� ���� ��������� ������������ delete ��� ������ ������������
			FinalRelease();
			//CVirtualConsole* pVCon = (CVirtualConsole*)this;
			//delete pVCon;
			return 0;
		}

		return mn_RefCount;
	};
protected:
	virtual ~CRefRelease()
	{
		_ASSERTE(mn_RefCount==REF_FINALIZE);
	};
};
