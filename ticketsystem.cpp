#include "ticketsystem.h"
#include "TrackModel/station_tracking.h"
#include "TrackModel/tracklayout.hpp"

TicketSystem *ticketSystem;

const char *Passenger::firstNames[]
{
    "Morgen", "Katy", "Chris", "Meara", "Maria", "Kirah", "Megan",
    "Cecil", "Carlos", "Abby", "Steve", "Janice",
    "Dana", "Earl", "Kevin", "Lauren", "Hiram",
    "Tamika", "Maureen", "Michelle", "John", "Diane",
    "Deb", "Josie", "Erika", "Pamela", "Marcus",
    "Leonard", "Louie", "Khoshekh", "Anna", "Melody",
    "Sheryl", "Fred", "Kaylin", "Felicity", "Clark",
    "Alma", "Rhianna", "David", "Kirsten", "Brian",
    "Angelica", "Charles", "Nina", "Trixie", "Gloria",
};

const int Passenger::N_FIRSTNAME = sizeof(Passenger::firstNames) / sizeof(char*);

const char *Passenger::lastNames[]
{
    "Benninghoff", "Fox", "Keach", "Murphy", "Mysliwiec", "Strandquist", "Toner",
    "Palmer", "Carlsberg", "Cardinal", "Harlan", "McDaniels",
    "Flynn", "Nguyen", "Peters", "Crayton", "Mallard",
    "Vanston", "Burton", "Blasko", "Janson", "Bonner",
    "Graves", "Miller", "Smith", "Johnson", "Williams",
    "Jones", "Brown", "Garcia", "Rodriguez", "Martinez",
    "Davis", "Hernandez", "Lopez", "Gonzales", "Wilson",
    "Anderson", "Thomas", "Taylor", "Moore", "Jackson",
    "Martin", "Lee", "Perez", "Thompson", "White"
};
const int Passenger::N_LASTNAME = sizeof(Passenger::lastNames) / sizeof(char*);


QString Passenger::createName()
{
    int firstIdx = rand() % N_FIRSTNAME;
    int lastIdx = rand() % N_LASTNAME;

    return QString("%1 %2").arg(firstNames[firstIdx]).arg(lastNames[lastIdx]);
}

TicketSystem::TicketSystem(QObject *parent) : QObject(parent)
{

}

void TicketSystem::clearSales()
{
    soldTickets.clear();
    emit ticketsChanged();
}

void TicketSystem::sellOneTicket(Route *route, Station *station, QDateTime time)
{
    QString name = Passenger::createName();
    soldTickets.push_back(Passenger(name, time, route, station));

    emit ticketsChanged();
}

void TicketSystem::sellTickets( Route *route, Station *station, QDateTime time, int count )
{
    if( count <= 0 ) return;

    for( int i = count; i > 0; i-- )
    {
        QString name = Passenger::createName();
        soldTickets.push_back(Passenger(name, time, route, station));
    }

    TrackModel::addPassengersToStation(route->name, station->name, count);

    emit ticketsChanged();
}

void TicketSystem::onTimeUpdate(const QDateTime &newTime, qint64 deltaMs)
{
    msSinceUpdate += deltaMs;
    if( msSinceUpdate >= UPDATE_DELAY )
    {
        // let's produce some humans
        for( Route *r : routes )
        {
            for( Station *s : r->stations )
            {
                int currentWaiting = getPassengersWaiting(r->name, s->name);

                if( currentWaiting >= SOFT_PASSENGER_CAP ) continue;
                else
                {
                    int arrivalCount = std::round(arrivalCurve(randEngine));
                    qint64 timeFudge = (rand() % UPDATE_DELAY) - UPDATE_DELAY; // pick a random time between last update & now
                    sellTickets(r, s, newTime.addMSecs(timeFudge), arrivalCount);
                }
            }
        }

        msSinceUpdate = 0;
    }
}
