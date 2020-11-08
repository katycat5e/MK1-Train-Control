#include "CTCBlockDisplay.h"
#include <unordered_set>
#include <QPainter>

CTCBlockDisplay::CTCBlockDisplay(QWidget *parent) : QWidget(parent), yardStat(yard)
{

}

void CTCBlockDisplay::setRoute( CTCRouteStatus *route )
{
    if( route == this->route ) return;

    this->route = route;
    blocks.clear();
    switches.clear();
    links.clear();

    struct ExistBlock
    {
        QPoint topLeft;
        BlockDir orient;
        ExistBlock() : orient(BLK_NODIR) {}
        ExistBlock( QPoint tl, BlockDir dir ) : topLeft(tl), orient(dir) {}
    };

    const int n_rows = 10;
    std::pair<CTCBlockStatus *, BlockDir> curBlocks[n_rows];
    std::pair<CTCBlockStatus *, BlockDir> nextColumn[n_rows];
    std::unordered_map<int, ExistBlock> searchedNodes;

    int branchRow = n_rows / 2;
    int left = BLOCK_LENGTH / 2;
    int top = ROW_OFFSET;

    for( int i = 0; i < n_rows; i++ )
    {
        curBlocks[i] = {nullptr, BLK_NODIR};
        nextColumn[i] = {nullptr, BLK_NODIR};
    }

    BlockRepr yardRepr(left, top, BLK_FORWARD, &yardStat);
    blocks.insert({0, yardRepr});
    int lnY = top + BLOCK_THICKNESS / 2;
    links.push_back(LinkRepr(left + BLOCK_LENGTH, lnY, left + BLOCK_LENGTH + LINK_WIDTH, lnY));

    searchedNodes.insert({0, ExistBlock(QPoint(left, top), BLK_FORWARD)});

    left += BLOCK_LENGTH + LINK_WIDTH;

    int startId = route->layoutRoute->spawnBlock->id;
    curBlocks[0] = {route->getBlockStatus(startId), BLK_FORWARD};
    searchedNodes.insert({startId, ExistBlock(QPoint(left, top), BLK_FORWARD)});
    bool hasNext = true;

    while( hasNext )
    {
        hasNext = false;

        for( int row = 0; row < n_rows; row++ )
        {
            CTCBlockStatus *curBlk = curBlocks[row].first;
            if( curBlk )
            {
                top = ROW_OFFSET * (row + 1);

                BlockRepr newRep(left, top, curBlocks[row].second, curBlk);
                blocks.insert({curBlocks[row].first->id(), newRep});

                Linkable *l = curBlk->layoutBlock->getLink(curBlocks[row].second);
                if( Switch *s = dynamic_cast<Switch *>(l) )
                {
                    // process straight branch
                    Block *straight = s->straightBlock;
                    if( straight->id )
                    {
                        if( searchedNodes.find(straight->id) == searchedNodes.end() )
                        {
                            // didn't process this block
                            CTCBlockStatus *sStat = route->getBlockStatus(straight->id);
                            BlockDir entryDir = curBlk->layoutBlock->getEntryDir(straight);
                            nextColumn[row] = {sStat, entryDir};
                            hasNext = true;

                            // record as searched
                            QPoint nextPt(left + BLOCK_LENGTH + LINK_WIDTH, top);
                            searchedNodes[straight->id] = ExistBlock(nextPt, entryDir);
                        }
                    }

                    Block *diverge = s->divergeBlock;
                    if( diverge->id && (searchedNodes.find(diverge->id) == searchedNodes.end()) )
                    {
                        if( searchedNodes.find(diverge->id) == searchedNodes.end() )
                        {
                            // didn't process this block
                            CTCBlockStatus *sStat = route->getBlockStatus(diverge->id);
                            BlockDir entryDir = curBlk->layoutBlock->getEntryDir(diverge);

                            nextColumn[branchRow] = {sStat, entryDir};
                            int rowSpan = (branchRow - row) * ROW_OFFSET;

                            // record as searched
                            QPoint nextPt(left + BLOCK_LENGTH + LINK_WIDTH, top + rowSpan);
                            searchedNodes[diverge->id] = ExistBlock(nextPt, entryDir);

                            branchRow -= 1;
                            if( branchRow == 0 ) branchRow = n_rows - 1;
                        }
                    }
                }
                else if( l )
                {
                    Block *next = l->getTarget();
                    if( next->id )
                    {
                        auto other = searchedNodes.find(next->id);
                        if( other != searchedNodes.end() )
                        {
                            // processed block
                            BlockDir entryDir = curBlk->layoutBlock->getEntryDir(next);

                            if( Switch *s = dynamic_cast<Switch *>(next->getLink(oppositeDir(entryDir))) )
                            {
                                // backwards switch
                            }
                            else
                            {
                                // Direct link
                                LinkRepr ln(left + BLOCK_LENGTH, top + BLOCK_THICKNESS / 2, 0, other->second.topLeft.y() + BLOCK_THICKNESS / 2);
                                if( entryDir == other->second.orient ) ln.endX = other->second.topLeft.x();
                                else ln.endX = other->second.topLeft.x() + BLOCK_LENGTH;

                                links.push_back(ln);
                            }
                        }
                        else
                        {
                            // didn't process this block
                            CTCBlockStatus *nStat = route->getBlockStatus(next->id);
                            BlockDir entryDir = curBlk->layoutBlock->getEntryDir(next);
                            nextColumn[row] = {nStat, entryDir};

                            // record as searched
                            QPoint nextPt(left + BLOCK_LENGTH + LINK_WIDTH, top);
                            searchedNodes[next->id] = ExistBlock(nextPt, entryDir);

                            int y = top + BLOCK_THICKNESS / 2;
                            LinkRepr ln(left + BLOCK_LENGTH, y, left + BLOCK_LENGTH + LINK_WIDTH, y);
                            links.push_back(ln);

                            hasNext = true;
                        }
                    }
                }
            }

            curBlocks[row] = nextColumn[row];
            nextColumn[row] = {nullptr, BLK_NODIR};
        }

        left += BLOCK_LENGTH + LINK_WIDTH;
    }

    for( auto &kvp : route->layoutRoute->switches )
    {
        Block *startBlk = kvp.second->fromBlock;
        ExistBlock &e = searchedNodes.at(startBlk->id);

        // switch source
        QPoint start;
        if( kvp.second->fromBlockDir == e.orient )
        {
            start = QPoint(e.topLeft.x() + BLOCK_LENGTH, e.topLeft.y() + BLOCK_THICKNESS / 2);
        }
        else
        {
            start = QPoint(e.topLeft.x(), e.topLeft.y() + BLOCK_THICKNESS / 2);
        }

        // straight
        QPoint stDest;
        Block *straight = kvp.second->straightBlock;
        BlockDir straightEntry = startBlk->getEntryDir(straight);
        e = searchedNodes.at(straight->id);
        if( straightEntry != e.orient )
        {
            stDest = QPoint(e.topLeft.x() + BLOCK_LENGTH, e.topLeft.y() + BLOCK_THICKNESS / 2);
        }
        else stDest = QPoint(e.topLeft.x(), e.topLeft.y() + BLOCK_THICKNESS / 2);

        // diverge
        QPoint divDest;
        Block *diverge = kvp.second->divergeBlock;
        BlockDir divEntry = startBlk->getEntryDir(diverge);
        e = searchedNodes.at(diverge->id);
        if( divEntry != e.orient )
        {
            divDest = QPoint(e.topLeft.x() + BLOCK_LENGTH, e.topLeft.y() + BLOCK_THICKNESS / 2);
        }
        else divDest = QPoint(e.topLeft.x(), e.topLeft.y() + BLOCK_THICKNESS / 2);

        SwitchRepr sr(start, kvp.second);
        sr.straightEnd = stDest;
        sr.divEnd = divDest;

        switches.insert({kvp.first, sr});
    }

    setMinimumSize(left + BLOCK_LENGTH + LINK_WIDTH * 2, ROW_OFFSET * (n_rows + 2));
    setGeometry(0, 0, left + BLOCK_LENGTH + LINK_WIDTH * 2, ROW_OFFSET * (n_rows + 2));
}

static void createDirCap( QPoint pts[], int x, int y, int width, int height )
{
    pts[0] = QPoint(x - width / 2, y);
    pts[1] = QPoint(x + width / 2, y - height / 2);
    pts[2] = QPoint(x + width / 2, y + height / 2);
    //pts[3] = QPoint(x, y + height);
}

void CTCBlockDisplay::drawSignals( BlockRepr &repr, QPainter *painter )
{
    static QColor sigColors[3]
    {
        SIG_RED, SIG_YELLOW, SIG_GREEN
    };

    const int RADIUS = BLOCK_THICKNESS * 3 / 4;
    QPoint center(repr.left, repr.top - BLOCK_THICKNESS);

    painter->setPen(Qt::NoPen);
    QColor &col = sigColors[repr.stat->getSignal(oppositeDir(repr.orient))];
    painter->setBrush(col);
    painter->drawEllipse(center, RADIUS, RADIUS);

    center.setX(repr.left + BLOCK_LENGTH);
    col = sigColors[repr.stat->getSignal(repr.orient)];
    painter->setBrush(col);
    painter->drawEllipse(center, RADIUS, RADIUS);
}

void CTCBlockDisplay::drawBlock( BlockRepr &repr, QPainter *painter )
{
    QRect outline(repr.left, repr.top, BLOCK_LENGTH, BLOCK_THICKNESS);

    const int TEXT_THICK = (ROW_OFFSET - BLOCK_THICKNESS) / 3;

    if( !repr.stat->id() )
    {
        // yard
        painter->fillRect(outline, YARD_COLOR);

        painter->setPen(QPen(LINK_COLOR, LINK_THICK));
        QRect textOutline(repr.left, repr.top + BLOCK_THICKNESS, BLOCK_LENGTH, TEXT_THICK);
        painter->drawText(textOutline, Qt::AlignHCenter | Qt::AlignTop, "Yard");
        return;
    }

    QColor fillCol;
    if( repr.stat->isOccupied() ) fillCol = OCC_COLOR;
    else fillCol = UNNOCC_COLOR;

    BlockDir ow = repr.stat->layoutBlock->oneWay;
    painter->setPen(Qt::NoPen);
    painter->setBrush(fillCol);
    if( ow == repr.orient )
    {
        // pointing right
        QPoint pts[3];
        createDirCap(pts, repr.left + BLOCK_LENGTH, repr.top + BLOCK_THICKNESS / 2, -BLOCK_LENGTH / 5, BLOCK_THICKNESS * 2);
        painter->drawPolygon(pts, 3);
    }
    else if( oppositeDir(ow) == repr.orient )
    {
        // pointing left
        QPoint pts[3];
        createDirCap(pts, repr.left, repr.top + BLOCK_THICKNESS / 2, BLOCK_LENGTH / 5, BLOCK_THICKNESS * 2);
        painter->drawPolygon(pts, 3);
    }

    painter->fillRect(outline, fillCol);

    // Station
    painter->setPen(QPen(LINK_COLOR, LINK_THICK));

    PlatformData& platform = repr.stat->layoutBlock->platform;
    if( platform.exists() )
    {
        outline = QRect(repr.left + BLOCK_LENGTH / 5, repr.top - BLOCK_THICKNESS, (BLOCK_LENGTH * 3) / 5, BLOCK_THICKNESS);
        painter->fillRect(outline, STATION_COLOR);

        QRect statTextOut(repr.left - LINK_WIDTH / 2, repr.top - BLOCK_THICKNESS - TEXT_THICK, BLOCK_LENGTH + LINK_WIDTH, TEXT_THICK);
        painter->drawText(statTextOut, Qt::AlignHCenter | Qt::AlignBottom, QString::fromStdString(platform.station->name));

        //Passenger Numbers
        int numPass = repr.stat->passnum;
        statTextOut= QRect(repr.left, repr.top - BLOCK_THICKNESS - (TEXT_THICK*2), BLOCK_LENGTH, TEXT_THICK);
        painter->drawText(statTextOut, Qt::AlignHCenter | Qt::AlignBottom, QString::number(numPass));
    }


    // block num
    QRect textOutline(repr.left, repr.top + BLOCK_THICKNESS, BLOCK_LENGTH, TEXT_THICK);
    painter->drawText(textOutline, Qt::AlignHCenter | Qt::AlignTop, QString::number(repr.stat->id()));

    // faults
    BlockFault faults = repr.stat->getFaults();
    if( faults != FAULT_NONE )
    {
        painter->setPen(QPen(FAULT_COLOR, LINK_THICK));
        textOutline = QRect(repr.left, repr.top + BLOCK_THICKNESS + TEXT_THICK, BLOCK_LENGTH, TEXT_THICK);
        painter->drawText(textOutline, Qt::AlignHCenter | Qt::AlignTop, getFaultAbbrev(faults));
    }

    drawSignals(repr, painter);
}

void CTCBlockDisplay::drawSwitch( SwitchRepr &repr, QPainter *painter )
{
    // draw active links
    painter->setPen(QPen(SWITCH_ACTIVE_COLOR, LINK_THICK));
    if( repr.stat->direction == SW_DIVERGING )
    {
        painter->drawLine(repr.start, repr.divEnd);
    }
    else
    {
        painter->drawLine(repr.start, repr.straightEnd);
    }

    // draw inactive links
    painter->setPen(QPen(NOLINK_COLOR, LINK_THICK));
    if( repr.stat->direction == SW_DIVERGING )
    {
        painter->drawLine(repr.start, repr.straightEnd);
    }
    else
    {
        // straight
        painter->drawLine(repr.start, repr.divEnd);
    }
}

void CTCBlockDisplay::paintEvent( QPaintEvent *event )
{
    QPainter painter(this);

    painter.setPen(QPen(LINK_COLOR, LINK_THICK));
    painter.fillRect(rect(), QColor(20, 20, 20));

    for( auto &ln : links )
    {
        painter.drawLine(ln.startX, ln.startY, ln.endX, ln.endY);
    }

    for( auto &sw : switches )
    {
        SwitchRepr &repr = sw.second;
        drawSwitch(repr, &painter);
    }

    painter.setPen(QPen(LINK_COLOR, LINK_THICK));
    for( auto &blk : blocks )
    {
        BlockRepr &repr = blk.second;
        drawBlock(repr, &painter);
    }
}
