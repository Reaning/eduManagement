#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<map>
using namespace std;
class User {
	string number;      //ѧ�� ֻ����ѧ�Ż��߹��ŵ�¼
	string password;
};
class Student : public User
{
public:
    string name;
    string grade;      //�꼶
    string college;  //ѧԺ
    string major;    //רҵ
    string course;   //ѧ��ȫ���γ���Ϣ���� �γ�����/�ڿεص�/�ڿ�ʱ��/�ڿ���ʦ����/�ɼ�&�γ�����/�ڿεص�/.....
    Student() {;}
    Student(string Number,string Password,string Name,string Grade,string College,string Major,string course="\0")
    {
        number=Number,password=Password,name=Name,grade=Grade,college=College,major=Major,this->course=course;
    }
    void transCourse(const Course & x) ;     //���γ��������Ϣ��ȡ������������course������
    void set_number(string number) {this->number=number;}
    void set_pass(string password) {this->password=password;}
    void set_name(string password) {this->name=name;}
    void set_grade(string grade) {this->grade=grade;}
    void set_major(string major) {this->major=major;}
    void set_course(string course) {this->course=course;}
    void set_college(string college) {this->college=college;}
};

void Student::transCourse(const Course & x)
{
    course+=x.cou_name+"/"+x.position+"/"+x.time+"/"+x.tea_name+"/"+x.mark+"&";
}

class Teacher : public User
{
public:
    string name;   //����
    string course;    //���̿γ����ֵļ��� ��ʽΪ �γ�����&�γ�����&...
    Teacher() {;}
    Teacher(string Number,string Password,string Name,string course="\0")
    {
        number=Number,password=Password,name=Name,this->course=course;
    }
    void transCourse(const Course & x) {course+=x.cou_name+"&";}     //���γ��������Ϣ��ȡ������������course������
    void set_number(string number) {this->number=number;}
    void set_pass(string password) {this->password=password;}
    void set_name(string password) {this->name=name;}
    void set_course(string course) {this->course=course;}
};

class Admini :public User      //����Ա
{
public:
    string name;        //����
};

class Find       //�������ֵ��
{
public:
    string key;
    string value;
    Find(){}
    Find(string a,string b)
    {
        key=a,value=b;
    }
};

class Course       //�洢�ڿγ��ļ��еĿγ���
{
public:
    string cou_name;       //�γ�����
    string tea_name;       //�ڿ���ʦ����
    string stu_name;      //ѧ������
    string stu_number;    //ѧ��ѧ��
    string position;       //�ڿεص�
    string time;          //�ڿ�ʱ��
    string mark;         //ѧ���ɼ�
};

/*----���ܺ���-----*/
//��ȡ������Ϣ
void openFind(string file_name,multimap<string,string> & x)
{
    fstream f(file_name,ios::binary|ios::in);
    f.seekg(0,ios::end);
    long pos=f.tellg();    //�õ��ļ�����ָ��λ��
    Find temp;
    for(f.seekg(0,ios::beg);f.tellg()!=pos;)
    {
        f.read((char*)&temp,sizeof(temp));
        x.insert(make_pair(temp.key,temp.value));     //��������Ϣ��������
    }
    f.close();
}

//����ѧ�Ż��������ҵ���Ӧλ�ò���ȡ��Ϣ
//��ѧ�Ż�������������ʱ������0�����򷵻�1
template<typename T>
int copyInfo(string info,multimap<string,string> & x,vector<T> & y)    //yΪ��Ҫ��ȡ����Ϣ;��vector����ԭ���Ƿ�ֹ�ж������
{
    int num=x.count(info);
    if(num==0) return 0;
    multimap<string,string>::iterator it=x.find(info);
    for(int i=0;i<num;i++)
    {
        string posi=(*it).second;    //�õ���Ϣ��ƫ����,�ļ����Ե�ַ&ƫ����
        string a(posi,0,posi.find('&')),b(posi,posi.find('&')+1,posi.size()-posi.find('&')-1);
        fstream f(a,ios::binary|ios::in);
        f.seekg(long(sizeof(T)*atoi(b.c_str())),ios::beg);      //ƫ�Ƶ���Ӧλ��
        T temp;
        f.read((char*)&temp,sizeof(T));      //��ȡ��Ϣ
        y.push_back(temp);
        it++;
        f.close();
    }
    return 1;
}

//����ѧ�Ż������������޸ĺ��yд���Ӧλ��
//����������ѧ�Ų�����ʱ������0�����򷵻�1��
template<typename T>
int modifyInfo(string info,multimap<string,string> & x,const T & y)
{
    if(x.count(info)==0) return 0;
    multimap<string,string>::iterator it=x.find(info);
    string posi=(*it).second;    //�õ���Ϣ��ƫ����,�ļ����Ե�ַ&ƫ����
    string a(posi,0,posi.find('&')),b(posi,posi.find('&')+1,posi.size()-posi.find('&')-1);
    fstream f(a,ios::binary|ios::out);
    f.seekp(long(sizeof(T)*atoi(b.c_str())),ios::beg);
    f.write((char*)&y,sizeof(T));
    f.close();
    return 1;
}

//����ѧ�Ż�������ɾ��ĳ�����������Ϣ
template<typename T>
void deleteInfo(string info,multimap<string,string> & x,string file_name)
{
    fstream f(file_name,ios::binary|ios::out|ios::trunc);
    x.erase(info);
    multimap<string,string>::iterator it=x.begin();    //���޸ĺ��������Ϣ����д���ļ�
    for(;it!=x.end();it++)
    {
        Find temp((*it).first,(*it).second);
        f.write((char*)&temp,sizeof(temp));
    }
    f.close();         //������Ϣ���ļ���ɾ�����
}

//������Ϣ��ֱ�����ļ���׷��,�����ظ���Ϣƫ�������������Ϣǰ���Ѿ���17����Ϣ���򷵻�18��
template<typename T>
int addInfo(const T & info,string file_name)
{
    fstream f(file_name,ios::out|ios::binary|ios::app);
	f.write((char*)&info, sizeof(T));
	int value = ((long)f.tellp() - ios::beg) / sizeof(T);
	f.close();
	return value;
}

//����������Ϣ
void addIndex(string key,string value,multimap<string,string> & x,string file_name)     //file_nameΪ�����ļ�
{
    x.insert(make_pair(key,value));
    Find temp(key,value);
    fstream f(file_name,ios::binary|ios::app|ios::out);
    f.write((char*)&temp,sizeof(temp));
    f.close();
}

//����ѧ������ʦ������Ա��Ա��Ϣ
//��Ҫ�������ļ��������ļ���׷����Ϣ
//��Ҫ�ṩ��Ա��Ϣ����Ӧ������������Ա��Ϣ�ļ�����������Ϣ�ļ���
//������������Ϊ��������ѧ��Ϊ�������ļ���������Ҫ���������ļ�
template<typename T>
void addPerson(const T & info,multimap<string,string> x2,multimap<string,string> x3,string file_name1,string file_name2,string file_name3)
{
    int num=addInfo(info,file_name1);   //�õ�ƫ����
    addIndex(info.name,file_name1+"&"+to_string(num),x2,file_name2);
    addIndex(info.number,file_name1+"&"+to_string(num),x3,file_name3);
}

//ǰ�᣺ѧ��ѡ�γɹ�
//�ڶ�Ӧ�γ���������Ϣ�������¿γ�������Ϣ������ѧ��������Ϣ�������ļ�
//��Ҫ�ṩѧ��������Ϣ��ѧ���������󣬿γ����֣��γ��������󣬿γ���Ϣ�ļ���������Ϣ�ļ���
//������������Ϊ��������ѧ��Ϊ�������ļ���������Ҫ���������ļ�
void addCourse(const Student & info,multimap<string,string> x1,string cou_name,multimap<string,string>& x2,multimap<string,string> &x3,string file_name1,string file_name2.string file_name3)
{
    fstream f(file_name1,ios::in|ios::binary);    //���ļ�����ȡ�γ���Ϣ
    Course temp;
    f.read((char*)&temp,sizeof(Course)); f.close();
    temp.stu_name=info.name,temp.stu_number=info.number;
    info.transCourse(temp);   //�޸�ѧ���γ���Ϣ
    modifyInfo(info.number,x1,info);  //�޸ĵ�ѧ���ļ���
    int num=addInfo(temp,file_name1); //�޸Ŀγ��ļ���Ϣ
    addIndex(info.name,file_name1+"&"+to_string(num),x2,file_name2);
    addIndex(info.number,file_name1+"&"+to_string(num),x3,file_name3);
}


//�ж��˺��Ƿ���ڣ������򷵻����룬���򷵻ؿ��ַ���
//ͨ��ѧ�Ż��������ҵ�ѧ����Ϣ�������˺�������Ϣ
//������Ҫ�ṩ�û���¼���˺��Լ���Ӧ��multimap�������иú���ֻ����һ����������
template<typename T>
string openAccount(string ID,const multimap<string,string> & x) {
    int flag=x.count(ID);      //����˻��Ƿ����
    if(flag==0) return "\0";     //�˻�������ֱ�ӷ��ؿ��ַ���
    multimap<string,string>::iterator it=x.find(ID);
    string posi=(*it).second;    //�õ���Ϣ��ƫ����,�ļ����Ե�ַ&ƫ����
    string a(posi,0,posi.find('&')),b(posi,posi.find('&')+1,posi.size()-posi.find('&')-1);
    fstream f(a,ios::binary|ios::in);
    f.seekg(long(sizeof(T)*atoi(b.c_str())),ios::beg);      //ƫ�Ƶ���Ӧλ��
	T temp;
	f.read((char*)&temp,sizeof(T));   //��ȡ��Ϣ
	f.close();
	return temp.password;
}

//��ȡ�����ļ�����Ϣ
//�����ļ��е�����
template<typename T>
int copyAllInfo(string file_name,vector<T> & x)
{
    fstream f(file_name,ios::binary|ios::in);
    f.seekg(0,ios::end);
    long End=f.tellg();    //��ȡĩλ��
    f.seekg(0,ios::beg);
    for(;f.tellg()!=End;)
    {
        T temp;
        f.read((char*)&temp,sizeof(T));
        x.push_back(temp);
    }
    f.close();    //��ȡ��ȫ����Ϣ
}

/*
//��֤�˺�����
template<typename T>
int checkAccount(string password, vector<T>& y) {
	if (y.empty())return -1;
	if (y[0].password == password)return 1;
	return 0;
}
*/

class System
{
public:
	void load_interface();					//��½����
	void exit_system();					    //�˳�ϵͳ
	void load_account();					//��ȡ�˺�����
	void set_stu_account();					//����ѧ���˻�
	void enter_stu_account();				//ѧ����ݵ�½
	void student_functionshow();			//ѧ���û����ܽ���
	void clerk_functionshow();				//����Ա���ܽ���
	void set_clerk_account();				//���ý���Ա�˻�
	void enter_clerk_account();				//����Ա��ݵ�½
	void teacher_functionshow();			//��ʦ���ܽ���
	void set_teacher_account();				//���ý�ʦ�˻�
	void enter_teacher_account();			//��ʦ��ݵ�½
	void save_undst();						//����ѧ������
	void load_undst();						//��ȡѧ������
	void load_index();						//��ȡ����
	void change_password();					//�޸�����
	void createRecord();					//��������
	void editRecord();						//�޸ļ�¼
	void deleteRecord();					//ɾ����¼
	void searchRecord();					//���Ҽ�¼
};

//��ʦ����ѡ����Ϣ����>����Աͬ�⡪������������ѡ����Ϣ�������������ӽ�ʦѡ�ε���Ϣ��ѧ����Ϣ
//��ʦ����һ�ſγ̵�ͬʱ��Ӧ�þ����ļ�������Ͽγ���Ϣ
