#include "ScanManager.h"

void ScanManager::Scan(const string& path){	//�ȶ��ļ�ϵͳ�����ݿ�
	vector<string>	localdirs, localfiles;
	DirectoryList(path, localdirs, localfiles);
	std::set<string> localset;
	localset.insert(localdirs.begin(), localdirs.end());
	localset.insert(localfiles.begin(), localfiles.end());

	std::set<string> dbset;
	_datamgr.GetDocs(path, dbset);

	auto localit = localset.begin();
	auto dbit = dbset.begin();
	while (localit != localset.end() && dbit != dbset.end()){
		if (*localit < *dbit){	//������,���ݿ�û��->�����ݿ�����
			_datamgr.InsertDoc(path, *localit);	//�����ݿ����������ļ�
			++localit;
		}
		else if (*localit > *dbit){	//����û��,���ݿ���->ɾ�����ݿ��е�
			_datamgr.DeleteDoc(path, *dbit);	//ɾ�����ݿ��б���û�е��ļ�
			++dbit;
		}
		else{	//û�б仯������
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end()){	//��������û������
		_datamgr.InsertDoc(path, *localit);	//�����ݿ����������ļ�
		++localit;
	}
	while (dbit != dbset.end()){	//���ݿ�����û������
		_datamgr.DeleteDoc(path, *dbit);	//ɾ�����ݿ��б���û�е��ļ�
		++dbit;
	}
	for (const auto& subdirs : localdirs){	//�ݹ�ɨ��ȶ���Ŀ¼�е�����
		string subpath = path + '\\' + subdirs;
		Scan(subpath);
	}
}

