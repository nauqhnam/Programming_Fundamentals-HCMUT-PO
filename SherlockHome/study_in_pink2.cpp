#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

MapElement::MapElement(ElementType in_type)
{
    this->type = in_type;
};

MapElement::~MapElement(){};

ElementType MapElement::getType() const
{
    return this->type;
};
Path::Path()
    : MapElement(PATH){};
Wall::Wall()
    : MapElement(WALL){};
FakeWall::FakeWall(int in_req_exp)
    : MapElement(FAKE_WALL)
{
    this->req_exp = in_req_exp;
};
int FakeWall::getReqExp() const
{
    return this->req_exp;
};
Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
};

Position::Position(const string &str_pos)
{
    int pos1 = str_pos.find(',');
    int pos2 = str_pos.length() - 1;
    this->r = stoi(str_pos.substr(1, pos1 - 1));
    this->c = stoi(str_pos.substr(pos1 + 1, pos2 - pos1 - 1));
};

int Position::getRow() const
{
    return this->r;
};
int Position::getCol() const
{
    return this->c;
};
void Position::setRow(int r)
{
    this->r = r;
};
void Position::setCol(int c)
{
    this->c = c;
};

string Position::str() const
{
    return "(" + to_string(this->r) + "," + to_string(this->c) + ")";
};

bool Position::isEqual(int in_r, int in_c) const
{
    return this->r == in_r && this->c == in_c;
};
const Position Position::npos = Position(-1, -1);
Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
    : num_rows(num_rows), num_cols(num_cols)
{
    this->map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; ++i)
    {
        this->map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; ++j)
        {
            this->map[i][j] = new Path();
        }
    }
    for (int i = 0; i < num_walls; ++i)
    {
        this->map[array_walls[i].getRow()][array_walls[i].getCol()] = new Wall();
    }
    for (int i = 0; i < num_fake_walls; ++i)
    {
        int r = array_fake_walls[i].getRow();
        int c = array_fake_walls[i].getCol();
        int in_req_exp = (r * 257 + c * 139 + 89) % 900 + 1;
        this->map[r][c] = new FakeWall(in_req_exp);
    }
};
Map::~Map()
{
    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            delete this->map[i][j];
        }
        delete[] this->map[i];
    }
    delete[] this->map;
};
bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    int r = pos.getRow();
    int c = pos.getCol();
    if (r < 0 || r >= num_rows || c < 0 || c >= num_cols)
    {
        return false;
    }
    if (map[r][c]->getType() == WALL)
    {
        return false;
    }
    if (map[r][c]->getType() == FAKE_WALL)
    {
        if (mv_obj->getName() == "Watson")
        {
            if (dynamic_cast<Watson *>(mv_obj)->getExp() < dynamic_cast<FakeWall *>(map[r][c])->getReqExp())
            {
                return false;
            }
        }
    }
    return true;
};
MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
};

MovingObject::~MovingObject(){};

Position MovingObject::getCurrentPosition() const
{
    return this->pos;
};

int MovingObject::getManhattanDistance(const Position &pos1, const Position &pos2) const
{
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
};

Character::Character(int index, const string &name, const Position &pos, Map *map, int hp, int exp)
    : MovingObject(index, pos, map, name)
{
    this->hp = hp;
    this->exp = exp;
    this->turn = 0;
};
Character::~Character(){};
int Character::getHp() const
{
    return this->hp;
};
int Character::getExp() const
{
    return this->exp;
};
void Character::setHp(int hp)
{
    this->hp = hp;
};
void Character::setExp(int exp)
{
    this->exp = exp;
};

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, "Sherlock", init_pos, map, init_hp, init_exp)
{
    this->moving_rule = moving_rule;
};
Position Sherlock::getNextPosition()
{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int len = this->moving_rule.length();
    int i = this->turn % len;
    char ch = this->moving_rule[i];
    if (ch == 'U')
    {
        r -= 1;
    }
    else if (ch == 'D')
    {
        r += 1;
    }
    else if (ch == 'L')
    {
        c -= 1;
    }
    else if (ch == 'R')
    {
        c += 1;
    }
    return Position(r, c);
};

void Sherlock::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        this->pos = next_pos;
    }
    this->turn += 1;
};
string Sherlock::str() const
{
    string res = "Sherlock[index=";
    res += to_string(this->index);
    res += ";pos=";
    res += this->pos.str();
    res += ";moving_rule=";
    res += this->moving_rule;
    res += "]";
    return res;
};

string Sherlock::getName() const
{
    return "Sherlock";
}
Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, "Watson", init_pos, map, init_hp, init_exp)
{
    this->moving_rule = moving_rule;
};
Position Watson::getNextPosition()
{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int len = this->moving_rule.length();
    int i = this->turn % len;
    char ch = this->moving_rule[i];
    if (ch == 'U')
    {
        r -= 1;
    }
    else if (ch == 'D')
    {
        r += 1;
    }
    else if (ch == 'L')
    {
        c -= 1;
    }
    else if (ch == 'R')
    {
        c += 1;
    }
    return Position(r, c);
};
void Watson::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        this->pos = next_pos;
    }
    this->turn += 1;
};
string Watson::str() const
{
    string res = "Watson[index=";
    res += to_string(this->index);
    res += ";pos=";
    res += this->pos.str();
    res += ";moving_rule=";
    res += this->moving_rule;
    res += "]";
    return res;
};

string Watson::getName() const
{
    return "Watson";
}
Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
    : Character(index, "Criminal", init_pos, map, 0, 0)
{
    this->sherlock = sherlock;
    this->watson = watson;
};
Position Criminal::getNextPosition()
{
    int max_dist = 0;
    Position next_pos = Position::npos;
    int r = this->pos.getRow();
    int c = this->pos.getCol();

    // up
    Position temp = Position(r - 1, c);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition()) + getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist > max_dist)
        {
            max_dist = dist;
            next_pos = temp;
        }
    }

    // left
    temp = Position(r, c - 1);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition()) + getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist > max_dist)
        {
            max_dist = dist;
            next_pos = temp;
        }
    }

    // down
    temp = Position(r + 1, c);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition()) + getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist > max_dist)
        {
            max_dist = dist;
            next_pos = temp;
        }
    }

    // right
    temp = Position(r, c + 1);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition()) + getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist > max_dist)
        {
            max_dist = dist;
            next_pos = temp;
        }
    }

    return next_pos;
};

void Criminal::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        this->prev_position = this->pos;
        this->pos = next_pos;
        this->turn += 1;
    }
    
};

string Criminal::str() const
{
    string res = "Criminal[index=";
    res += to_string(this->index);
    res += ";pos=";
    res += this->pos.str();
    res += "]";
    return res;
};

string Criminal::getName() const
{
    return "Criminal";
};
Position Criminal::getPrevPos() const {
    return this->prev_position;
}
ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    this->arr_mv_objs = new MovingObject *[capacity];
};

ArrayMovingObject::~ArrayMovingObject()
{
    delete[] arr_mv_objs;
};
bool ArrayMovingObject::isFull() const
{
    return this->count == this->capacity;
};
bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (this->isFull())
    {
        return false;
    }
    this->arr_mv_objs[this->count] = mv_obj;
    this->count += 1;
    return true;
};
MovingObject *ArrayMovingObject::get(int index) const
{
    return this->arr_mv_objs[index];
};
int ArrayMovingObject::size() const
{
    return this->count;
};
string ArrayMovingObject::str() const
{
    string res = "ArrayMovingObject[";
    res += "count=";
    res += to_string(this->count);
    res += ";capacity=";
    res += to_string(this->capacity);
    res += ";";
    for (int i = 0; i < this->count; ++i)
    {
        res += this->arr_mv_objs[i]->str();
        if (i != this->count - 1)
        {
            res += ";";
        }
    }
    res += "]";
    return res;
};
Configuration::Configuration(const string &filepath)
{
    ifstream file(filepath);
    string line;
    string key;
    string value;
    while (getline(file, line))
    {
        if (line.length() == 0)
        {
            continue;
        }
        int pos = line.find('=');
        key = line.substr(0, pos);
        value = line.substr(pos + 1);
        if (key == "MAP_NUM_ROWS")
        {
            this->map_num_rows = stoi(value);
        }
        else if (key == "MAP_NUM_COLS")
        {
            this->map_num_cols = stoi(value);
        }
        else if (key == "MAX_NUM_MOVING_OBJECTS")
        {
            this->max_num_moving_objects = stoi(value);
        }
        else if (key == "ARRAY_WALLS")
        {
            // Count the number of walls
            this->num_walls = 1;
            for (char c : value)
            {
                if (c == ';')
                {
                    this->num_walls++;
                }
            }
            this->arr_walls = new Position[this->num_walls];
            int prevIndex = 1, wallIndex = 0;
            for (int i = 1; i <= value.length(); ++i)
            {
                if (i == value.length() || value[i] == ';')
                {
                    this->arr_walls[wallIndex] = Position(value.substr(prevIndex, i - prevIndex));
                    wallIndex++;
                    prevIndex = i + 1;
                }
            }
        }
        else if (key == "ARRAY_FAKE_WALLS")
        {
            this->num_fake_walls = 0;
            int prevIndex = 1;
            for (int i = 1; i < value.length(); i++)
            {
                if (value[i] == ';' || (i == value.length() - 1 && i - prevIndex > 4))
                {
                    this->num_fake_walls++;
                    if (this->arr_fake_walls != nullptr)
                    {
                        delete[] this->arr_fake_walls;
                    };
                    this->arr_fake_walls = new Position[this->num_fake_walls];
                    this->arr_fake_walls[this->num_fake_walls - 1] = Position(value.substr(prevIndex, i - prevIndex));
                }
            }
        }
        else if (key == "SHERLOCK_INIT_POS")
        {
            this->sherlock_init_pos = Position(value);
        }
        else if (key == "SHERLOCK_INIT_HP")
        {
            this->sherlock_init_hp = stoi(value);
        }
        else if (key == "SHERLOCK_INIT_EXP")
        {
            this->sherlock_init_exp = stoi(value);
        }
        else if (key == "SHERLOCK_MOVING_RULE")
        {
            this->sherlock_moving_rule = value;
        }
        else if (key == "WATSON_INIT_POS")
        {
            this->watson_init_pos = Position(value);
        }
        else if (key == "WATSON_INIT_HP")
        {
            this->watson_init_hp = stoi(value);
        }
        else if (key == "WATSON_INIT_EXP")
        {
            this->watson_init_exp = stoi(value);
        }
        else if (key == "WATSON_MOVING_RULE")
        {
            this->watson_moving_rule = value;
        }
        else if (key == "CRIMINAL_INIT_POS")
        {
            this->criminal_init_pos = Position(value);
        }
        else if (key == "NUM_STEPS")
        {
            this->num_steps = stoi(value);
        }
    };
};
Configuration::~Configuration()
{
    delete[] this->arr_walls;
    delete[] this->arr_fake_walls;
};
string Configuration::str() const
{
    string res = "Configuration[\n";
    res += "MAP_NUM_ROWS=";
    res += to_string(this->map_num_rows);
    res += "\nMAP_NUM_COLS=";
    res += to_string(this->map_num_cols);
    res += "\nMAX_NUM_MOVING_OBJECTS=";
    res += to_string(this->max_num_moving_objects);
    res += "\nNUM_WALLS=";
    res += to_string(this->num_walls);
    res += "\nARRAY_WALLS=[";
    for (int i = 0; i < this->num_walls; ++i)
    {
        res += this->arr_walls[i].str();
        if (i != this->num_walls - 1)
        {
            res += ";";
        }
    }
    res += "]\nNUM_FAKE_WALLS=";
    res += to_string(this->num_fake_walls);
    res += "\nARRAY_FAKE_WALLS=[";
    for (int i = 0; i < this->num_fake_walls; ++i)
    {
        res += this->arr_fake_walls[i].str();
        if (i != this->num_fake_walls - 1)
        {
            res += ";";
        }
    }
    res += "]\nSHERLOCK_MOVING_RULE=";
    res += this->sherlock_moving_rule;
    res += "\nSHERLOCK_INIT_POS=";
    res += this->sherlock_init_pos.str();
    res += "\nSHERLOCK_INIT_HP=";
    res += to_string(this->sherlock_init_hp);
    res += "\nSHERLOCK_INIT_EXP=";
    res += to_string(this->sherlock_init_exp);
    res += "\nWATSON_MOVING_RULE=";
    res += this->watson_moving_rule;
    res += "\nWATSON_INIT_POS=";
    res += this->watson_init_pos.str();
    res += "\nWATSON_INIT_HP=";
    res += to_string(this->watson_init_hp);
    res += "\nWATSON_INIT_EXP=";
    res += to_string(this->watson_init_exp);
    res += "\nCRIMINAL_INIT_POS=";
    res += this->criminal_init_pos.str();
    res += "\nNUM_STEPS=";
    res += to_string(this->num_steps);
    res += "\n]";
    return res;
};
StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
};

bool StudyPinkProgram::isStop() const
{
    return false;
}

void StudyPinkProgram::printResult() const
{
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
    {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol()))
    {
        cout << "Watson caught the criminal" << endl;
    }
    else
    {
        cout << "The criminal escaped" << endl;
    }
}

void StudyPinkProgram::printStep(int si) const
{
    cout << "Step: " << setw(4) << setfill('0') << si
         << "--"
         << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
}

void StudyPinkProgram::run(bool verbose)
{
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            arr_mv_objs->get(i)->move();
            if (isStop())
            {
                printStep(istep);
                break;
            }
            if (verbose)
            {
                printStep(istep);
            }
        }
    }
    printResult();
}

StudyPinkProgram::~StudyPinkProgram(){};
Robot::Robot(int index, const Position &init_pos, Map *map, RobotType type, Criminal *criminal)
    : MovingObject(index, init_pos, map, "Robot"), robot_type(type), criminal(criminal), item(nullptr) {}

Robot::~Robot() {}

RobotType Robot::getType() const
{
    return robot_type;
}

void Robot::setItem(BaseItem *new_item)
{
    item = new_item;
}
BaseItem* Robot::createItem(int i, int j) {
     int p = i * j;
    int s = calNum(p);
    int t = (i * 11 + j) % 4;

    std::string challengeType;
    switch (t) {
        case 0: challengeType = "RobotS"; break;
        case 1: challengeType = "RobotC"; break;
        case 2: challengeType = "RobotSW"; break;
        case 3: challengeType = "all"; break;
    }

    if (s <= 1) {
        return new MagicBook();
    } else if (s <= 3) {
        return new EnergyDrink();
    } else if (s <= 5) {
        return new FirstAid();
    } else if (s <= 7) {
        return new ExcemptionCard(challengeType);
    } else if (s <= 9) {
        return new PassingCard(challengeType);
    }

    return nullptr;
}
string Robot::getName() const
{
    return "Robot";
}
RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal)
    : Robot(index, init_pos, map, C, criminal) {}

Position RobotC::getNextPosition()
{
    return criminal->getPrevPos();
}

void RobotC::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        this->pos = next_pos;
    }
}

int RobotC::getDistance() const
{
    return 0;
}

string RobotC::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=C;dist=]";
}
string RobotC::getName() const
{
    return "RobotC";
}
RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock)
    : Robot(index, init_pos, map, S, criminal), sherlock(sherlock) {}

Position RobotS::getNextPosition()
{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int min_dist = INT_MAX;
    Position next_pos = Position::npos;

    // Check up
    Position temp = Position(r - 1, c);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    // Check left
    temp = Position(r, c - 1);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    // Check down
    temp = Position(r + 1, c);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    // Check right
    temp = Position(r, c + 1);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, sherlock->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    return next_pos;
}

void RobotS::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        this->pos = next_pos;
    }
}

int RobotS::getDistance() const
{
    return getManhattanDistance(this->pos, sherlock->getCurrentPosition());
}

string RobotS::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=S;dist=" + to_string(getDistance()) + "]";
}
RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson)
    : Robot(index, init_pos, map, W, criminal), watson(watson) {}

Position RobotW::getNextPosition()
{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int min_dist = INT_MAX;
    Position next_pos = Position::npos;

    //up
    Position temp = Position(r - 1, c);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    //left
    temp = Position(r, c - 1);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    //down
    temp = Position(r + 1, c);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    //right
    temp = Position(r, c + 1);
    if (map->isValid(temp, this))
    {
        int dist = getManhattanDistance(temp, watson->getCurrentPosition());
        if (dist < min_dist)
        {
            min_dist = dist;
            next_pos = temp;
        }
    }

    return next_pos;
}

void RobotW::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        this->pos = next_pos;
    }
}

int RobotW::getDistance() const
{
    return getManhattanDistance(this->pos, watson->getCurrentPosition());
}

string RobotW::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
}
RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
    : Robot(index, init_pos, map, SW, criminal), sherlock(sherlock), watson(watson) {}

Position RobotSW::getNextPosition()
{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int min_dist = INT_MAX;
    Position next_pos = Position::npos;
    for (int dr = -2; dr <= 2; ++dr)
    {
        for (int dc = -2; dc <= 2; ++dc)
        {
            if (abs(dr) + abs(dc) == 2)
            {
                Position temp = Position(r + dr, c + dc);
                if (map->isValid(temp, this))
                {
                    int dist = getManhattanDistance(temp, sherlock->getCurrentPosition()) +
                               getManhattanDistance(temp, watson->getCurrentPosition());
                    if (dist < min_dist)
                    {
                        min_dist = dist;
                        next_pos = temp;
                    }
                }
            }
        }
    }

    return next_pos;
}

void RobotSW::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this))
    {
        this->pos = next_pos;
    }
}

int RobotSW::getDistance() const
{
    return getManhattanDistance(this->pos, sherlock->getCurrentPosition()) +
           getManhattanDistance(this->pos, watson->getCurrentPosition());
}

string RobotSW::str() const
{
    return "Robot[pos=" + this->pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";
}
//dlbvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
BaseBag::~BaseBag() {
    NodeItem *current = head;
    while (current != nullptr) {
        NodeItem *next = current->next;
        delete current;
        current = next;
    }
}

bool BaseBag::insert(BaseItem *item) {
    if (count >= capacity) return false;
    NodeItem *newNode = new NodeItem(item);
    newNode->next = head;
    head = newNode;
    count++;
    return true;
}

BaseItem* BaseBag::get() {
    if (head == nullptr) return nullptr;
    NodeItem *node = head;
    head = head->next;
    BaseItem *item = node->item;
    delete node;
    count--;
    return item;
}

BaseItem* BaseBag::get(ItemType itemType) {
    NodeItem *current = head;
    NodeItem *prev = nullptr;

    while (current != nullptr) {
        if (current->item->getType() == itemType) {
            if (prev != nullptr) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            BaseItem *item = current->item;
            delete current;
            count--;
            return item;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

string BaseBag::str() const {
    string result;
    NodeItem *current = head;
    while (current != nullptr) {
        result += current->item->str() + "\n";
        current = current->next;
    }
    return result;
}

SherlockBag::SherlockBag(Sherlock* sherlock) {
    this->obj = sherlock;
    this->capacity = 13;
}

WatsonBag::WatsonBag(Watson* watson) {
    this->obj = watson;
    this->capacity = 15;
}
bool MagicBook::canUse(Character* obj, Robot* robot) {
    return obj->getExp() <= 350;
}
void MagicBook::use(Character* obj, Robot* robot) {
    obj->setExp(obj->getExp() * 1.25);
}

bool EnergyDrink::canUse(Character* obj, Robot* robot) {
    return obj->getHp() <= 100;
}
void EnergyDrink::use(Character* obj, Robot* robot) {
    obj->setHp(obj->getHp() * 1.2);
}

bool FirstAid::canUse(Character* obj, Robot* robot) {
    return obj->getHp() <= 100 || obj->getExp() <= 350;
}
void FirstAid::use(Character* obj, Robot* robot) {
    obj->setHp(obj->getHp() * 1.5);
}


ExcemptionCard::ExcemptionCard(const std::string& challenge) : challenge(challenge) {}
bool ExcemptionCard::canUse(Character* obj, Robot* robot) {
    return obj->getName() == "Sherlock" && obj->getHp() % 2 != 0;
}
void ExcemptionCard::use(Character* obj, Robot* robot) {
    
}

PassingCard::PassingCard(const std::string& challenge) : challenge(challenge) {}
bool PassingCard::canUse(Character* obj, Robot* robot) {
    return obj->getName() == "Watson" && obj->getHp() % 2 == 0;
}
void PassingCard::use(Character* obj, Robot* robot) {
    if (challenge == "all" || robot->getType() == getRobotTypeFromChallenge(challenge)) {
    } else {
        obj->setExp(obj->getExp() - 50);
    }
}
RobotType PassingCard::getRobotTypeFromChallenge(const std::string& challenge) {
    if (challenge == "RobotS") return S;
    if (challenge == "RobotC") return C;
    if (challenge == "RobotSW") return SW;
    return C; // Default case
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////