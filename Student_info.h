#ifndef GUARD_student_info_h
#define GUARD_student_info_h

#include <stdexcept> // runtime_error
#include <iostream>
#include <vector>
#include <string>

#include "Handle.h"


    /******************
    * BASE CORE CLASS *
    *******************/

class Core{
friend class Handle;
public:
    Core() : midterm(0), final(0) { }
    Core(std::istream& is) { read(is); }

    std::string name() const { return n; }
    virtual std::istream& read(std::istream&);
    virtual double grade() const;
    virtual bool checkRequirementsMet() const { return homework.size() != 0; }
    virtual std::string letterGrade() const;
protected:
    virtual Core* clone() const { return new Core(*this); }
    std::istream& read_exams(std::istream&);
    std::istream& read_name(std::istream&);
    double midterm, final;
    std::vector<double> homework;
private:
    std::string n;
};

    /*********************
    * DERIVED GRAD CLASS *
    **********************/

class Grad: public Core {
public:
    Grad() : thesis(0) { }
    Grad(std::istream& is) { read(is); }

    std::istream& read(std::istream&); // inherits virtual-ness from Core::read()
    double grade() const; // inherits virtual-ness from Core::grade()
    bool checkRequirementsMet() const { return (homework.size() != 0 && thesis != 0); }
protected:
    Grad* clone() const { return new Grad(*this); } // Student_info can access this clone function through a virtual call of Core::clone
private:
    double thesis;
};

    /*************************
    * DERIVED PASSFAIL CLASS *
    **************************/
class PassOrFail: public Core {
public:
    PassOrFail() { }
    PassOrFail(std::istream& is) { read(is); }

    std::istream& read(std::istream&);
    double grade() const;
    bool checkRequirementsMet() const { return (midterm != 0 || final != 0); }
    std::string letterGrade() const;

protected:
    PassOrFail* clone() const { return new PassOrFail(*this); }
};


    /*************************
    * DERIVED AUDITING CLASS *
    **************************/
class Auditing : public Core{
public:
    Auditing() { }
    Auditing(std::istream& is) { read(is); }

    std::istream& read(std::istream&);
    bool checkRequirementsMet() const { return true; }
    std::string letterGrade() const;

protected:
    Auditing* clone() const { return new Auditing(*this); }
};

    /*************************************
    * STUDENT_INFO IMPLEMENTATION CLASS  *
    **************************************/

class Student_info{
public:
    Student_info() : cp(0) { }
    Student_info(std::istream& is) : cp(0) { read(is); }
    
    std::istream& read(std::istream&);
    std::string name() const{
        if(cp) return cp->name();
        else throw std::runtime_error("Cannot find name of uninitialized student");
    }
    double grade() const{
        if(cp) return cp->grade();
        else throw std::runtime_error("Cannot find grade of uninitialized student");
    }
    bool checkRequirementsMet() const{
        if(cp) return cp->checkRequirementsMet();
        else throw std::runtime_error("Cannot check uninitialized student");
    }
    std::string letterGrade() const{
        if(cp) return cp->letterGrade();
        else throw std::runtime_error("Cannot check letter grade of uninitialized student");
    }

    static bool compare(const Student_info& s1, const Student_info& s2){
        return ::compare(s1.name(), s2.name());
    }
private:
    Handle<Core> cp;
};

    /**********************************************
    * NON MEMBER FUNCTIONS AND TEMPLATE FUNCTIONS *
    ***********************************************/
bool compare(const std::string&, const std::string&);
bool compare_grades(const Core&, const Core&);
double grade(double, double, const std::vector<double>&);

template <class container>
std::istream& read_hw(std::istream& is, container& c){
    if (is){
        c.clear();
        double x;
        while (is.peek() != '\n'){
            is >> x;
            c.push_back(x);
        }

        is.clear();
    }
    return is;
};

template <class inputIter, class T>
void median(inputIter first, inputIter last, T& init){
    // make copy of vector to avoid altering the original vector
    std::vector<T> temp;
    while(first != last){
        temp.push_back(*first);
        ++first;
    }
    std::sort(temp.begin(), temp.end());

    // if even numbered, find the average of the middle two. If odd numbered, the median is the middle element.
    size_t size = temp.size();
    size_t mid = size / 2;
    size % 2 == 0 ? init = (temp[mid - 1] + temp[mid]) / 2 : init = temp[mid];
};


#endif // GUARD_student_info_h


