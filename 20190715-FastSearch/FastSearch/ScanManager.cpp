#include "ScanManager.h"

void ScanManager::Scan(const string& path){
	//�ȶ��ļ�ϵͳ�����ݿ�
	vector<string>	localdirs, localfiles;
	DirectoryList(path, localdirs, localfiles);

	std::set<string> localset;
	localset.insert(localdirs.begin(), localdirs.end());
	localset.insert(localfiles.begin(), localfiles.end());

	std::set<string> dbset;
	//DataManager()
	auto localit = localset.begin();
	auto dbit = dbset.begin();
	while (localit != localset.end() && dbit != dbset.end()){
		if (*localit < *dbit){	//������,���ݿ�û��->�����ݿ�����
			//����
			++localit;
		}
		else if (*localit > *dbit){	//����û��,���ݿ���->ɾ�����ݿ��е�
			//ɾ��
			++dbit;
		}
		else{	//û�б仯������
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end()){	//��������û������
		//����
		++localit;
	}
	while (dbit != dbset.end()){	//���ݿ�����û������
		//ɾ��
		++dbit;
	}
	for (const auto& subdirs : localdirs){	//�ݹ�ɨ��ȶ���Ŀ¼�е�����
		//Scan(subdirs);
	}
}

