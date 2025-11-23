/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Position;
class Configuration;
class Map;
class MovingObject;
class Watson;
class Sherlock;
class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

class MapElement
{
    friend class TestStudyInPink;

protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
    friend class TestStudyInPink;

public:
    Path();
};

class Wall : public MapElement
{
    friend class TestStudyInPink;

public:
    Wall();
};

class FakeWall : public MapElement
{
    friend class TestStudyInPink;

private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Position
{
    friend class TestStudyInPink;

private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0);

    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
};

class Map
{
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls);
    ~Map();
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

class MovingObject
{
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;

    int getManhattanDistance(const Position &pos1, const Position &pos2) const;
    virtual string getName() const = 0;
};

class Character : public MovingObject
{
    friend class TestStudyInPink;

protected:
    int hp, exp;
    int turn;

public:
    Character(int index, const string &name, const Position &pos, Map *map, int hp, int exp);
    virtual ~Character();
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    int getHp() const;
    int getExp() const;
    void setHp(int hp);
    void setExp(int exp);

    virtual string getName() const = 0;
};

class Sherlock : public Character
{
    friend class TestStudyInPink;

private:
    string moving_rule;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    string getName() const;
    void move();
    string str() const;
};

class Watson : public Character
{
    friend class TestStudyInPink;

private:
    string moving_rule;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;

    string getName() const;
};

class Criminal : public Character
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
    Watson *watson;
    Position prev_position;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();

    void move();

    string str() const;

    string getName() const;
    Position getPrevPos() const;
};

class ArrayMovingObject
{
    friend class TestStudyInPink;

private:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    MovingObject *get(int index) const;
    int size() const;
    string str() const;
};

class Configuration
{
    friend class StudyPinkProgram;

private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};



class StudyPinkProgram
{
    friend class TestStudyInPink;

private:
    
    Configuration *config;

    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;

    Map *map;
    ArrayMovingObject *arr_mv_objs;

public:
    StudyPinkProgram(const string &config_file_path);

    bool isStop() const;

    void printResult() const;

    void printStep(int si) const;
    void run(bool verbose);
    ~StudyPinkProgram();
    
};
// errorrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr

class Robot : public MovingObject
{
    friend class TestStudyInPink;
protected:
    RobotType robot_type;
    BaseItem *item;
    Criminal *criminal;
    static int calNum(int n) {
    while (n >= 10) {
        int sum = 0;
        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }
        n = sum;
    }
    return n;
}
   
public:

    Robot(int index, const Position &init_pos, Map *map, RobotType type, Criminal *criminal);
    Robot(int index, const Position& pos, Map* map, const string& name, RobotType robot_type)
        : MovingObject(index, pos, map, name), robot_type(robot_type) {
        this->item = createItem(pos.getRow(), pos.getCol());
    }

    virtual ~Robot();

    RobotType getType() const;

    void setItem(BaseItem *new_item);
    virtual int getDistance() const = 0;
    static BaseItem* createItem(int i, int j);
    virtual string getName() const;
   
};

class RobotC : public Robot
{
    friend class TestStudyInPink;
public:
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);
    Position getNextPosition();

    void move();

    int getDistance() const;

    string str() const;
    string getName() const override;
    
};

class RobotS : public Robot
{
    Sherlock *sherlock;
    friend class TestStudyInPink;
public:
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock);
    Position getNextPosition() override;

    void move() override;

    int getDistance() const override;

    string str() const override;
    
};

class RobotW : public Robot
{
    Watson *watson;
    friend class TestStudyInPink;
public:
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);
    Position getNextPosition();

    void move();

    int getDistance() const;

    string str() const;
   
};

class RobotSW : public Robot
{
    Sherlock *sherlock;
    Watson *watson;
    friend class TestStudyInPink;
public:
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();

    void move();

    int getDistance() const;

    string str() const;
    
};

class BaseItem
{
    friend class TestStudyInPink;

public:
    
    virtual ~BaseItem() {}
    virtual string str() const = 0;
    virtual ItemType getType() const =0;;

    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    
};
class NodeItem {
public:
    BaseItem* item;
    NodeItem* next;

    NodeItem(BaseItem* item) : item(item), next(nullptr) {}
};
class BaseBag {
protected:
    int capacity;
    int count = 0;
    Character *obj;
    NodeItem *head = nullptr;

public:
    friend class TestStudyPink;

    BaseBag() : capacity(0), count(0), obj(nullptr), head(nullptr) {}
    virtual ~BaseBag();

    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);
    virtual string str() const;
};

class SherlockBag : public BaseBag {
public:
    friend class TestStudyPink;

    SherlockBag(Sherlock *sherlock);
};

class WatsonBag : public BaseBag {
public:
    friend class TestStudyPink;

    WatsonBag(Watson *watson);
};



class MagicBook : public BaseItem
{
    friend class TestStudyInPink;
public:
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() const override { return "MagicBook"; }
    ItemType getType() const override { return ItemType::MAGIC_BOOK; }
    
};

class EnergyDrink : public BaseItem
{
    friend class TestStudyInPink;
public:
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
     string str() const override { return "EnergyDrink"; }
    ItemType getType() const override { return ItemType::ENERGY_DRINK; }
    
};

class FirstAid : public BaseItem
{
    friend class TestStudyInPink;
public:
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() const override { return "FirstAid"; }
    ItemType getType() const override { return ItemType::FIRST_AID; }
    
};

class ExcemptionCard : public BaseItem
{
    friend class TestStudyInPink;
private:
    string challenge;
public:
    ExcemptionCard(const string &challenge);
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
     string str() const override { return "ExcemptionCard"; }
    ItemType getType() const override { return ItemType::EXCEMPTION_CARD; }
    
};

class PassingCard : public BaseItem
{
    friend class TestStudyInPink;
private:
    string challenge;
    RobotType getRobotTypeFromChallenge(const string &challenge);

public:
    PassingCard(const string &challenge);
    bool canUse(Character *obj, Robot *robot) override;
    void use(Character *obj, Robot *robot) override;
    string str() const override { return "PassingCard"; }
    ItemType getType() const override { return ItemType::PASSING_CARD; }
    
};
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
