#include "GraphStyle.hxx"

#include "GraphData.hxx"
#include "Graph.hxx"

namespace systemtap
{
  using namespace std;
  using namespace tr1;

  GraphStyleBar GraphStyleBar::instance;
  
  void GraphStyleBar::draw(std::tr1::shared_ptr<GraphDataBase> graphData,
                           Graph* graph, Cairo::RefPtr<Cairo::Context> cr)
  {
    shared_ptr<GraphData<double> > realData
      = dynamic_pointer_cast<GraphData<double> >(graphData);
    if (!realData)
      return;
    int64_t left, right;
    double top, bottom;
    graph->getExtents(left, right, top, bottom);
    double horizScale = (graph->_zoomFactor * graph->_graphWidth
                         / static_cast<double>(right - left));
    GraphDataBase::TimeList::iterator lower
      = lower_bound(graphData->times.begin(), graphData->times.end(), left);
    GraphDataBase::TimeList::iterator upper
      = upper_bound(graphData->times.begin(), graphData->times.end(), right);
    for (GraphDataBase::TimeList::iterator ditr = lower, de = upper;
         ditr != de;
         ++ditr)
      {
        size_t dataIndex = ditr - graphData->times.begin();
        cr->set_source_rgba(graphData->color[0], graphData->color[1],
                            graphData->color[2], 1.0);
        cr->move_to((*ditr - left) * horizScale, 0);
        cr->line_to((*ditr - left) * horizScale,
                    realData->data[dataIndex] * graph->_graphHeight
                    / graphData->scale);
        cr->stroke();
      }
  }

  GraphStyleDot GraphStyleDot::instance;
  
  void GraphStyleDot::draw(std::tr1::shared_ptr<GraphDataBase> graphData,
                           Graph* graph, Cairo::RefPtr<Cairo::Context> cr)
  {
    shared_ptr<GraphData<double> > realData
      = dynamic_pointer_cast<GraphData<double> >(graphData);
    if (!realData)
      return;
    int64_t left, right;
    double top, bottom;
    graph->getExtents(left, right, top, bottom);
    double horizScale = (graph->_zoomFactor * graph->_graphWidth
                         / static_cast<double>(right - left));
    GraphDataBase::TimeList::iterator lower
      = lower_bound(graphData->times.begin(), graphData->times.end(), left);
    GraphDataBase::TimeList::iterator upper
      = upper_bound(graphData->times.begin(), graphData->times.end(), right);
    cr->set_source_rgba(graphData->color[0], graphData->color[1],
                        graphData->color[2], 1.0);

    for (GraphDataBase::TimeList::iterator ditr = lower, de = upper;
         ditr != de;
         ++ditr)
      {
        size_t dataIndex = ditr - graphData->times.begin();
        cr->arc((*ditr - left) * horizScale,
                (realData->data[dataIndex]
                 * graph->_graphHeight / graphData->scale),
                graph->_lineWidth / 2.0, 0.0, M_PI * 2.0);
        cr->fill();
      }
  }

  GraphStyleEvent GraphStyleEvent::instance;
  
  void GraphStyleEvent::draw(std::tr1::shared_ptr<GraphDataBase> graphData,
                             Graph* graph, Cairo::RefPtr<Cairo::Context> cr)
  {
        shared_ptr<GraphData<string> > stringData
      = dynamic_pointer_cast<GraphData<string> >(graphData);
    if (!stringData)
      return;
    int64_t left, right;
    double top, bottom;
    graph->getExtents(left, right, top, bottom);
    double horizScale = (graph->_zoomFactor * graph->_graphWidth
                         / static_cast<double>(right - left));
    double eventHeight = graph->_graphHeight * (graphData->scale / 100.0);
    cr->save();
    cr->set_line_width(3 * graph->_lineWidth);
    cr->set_source_rgba(graphData->color[0], graphData->color[1],
                        graphData->color[2], .33);
    cr->move_to(0, eventHeight);
    cr->line_to(graph->_graphWidth, eventHeight);
    cr->stroke();
    cr->restore();
    GraphDataBase::TimeList::iterator lower
      = lower_bound(graphData->times.begin(), graphData->times.end(), left);
    GraphDataBase::TimeList::iterator upper
      = upper_bound(graphData->times.begin(), graphData->times.end(), right);
    for (GraphDataBase::TimeList::iterator ditr = lower, de = upper;
         ditr != de;
         ++ditr)
      {
        size_t dataIndex = ditr - graphData->times.begin();
        double eventHeight = graph->_graphHeight * (graphData->scale / 100.0);
        cr->save();
        cr->select_font_face("Sans", Cairo::FONT_SLANT_NORMAL,
                             Cairo::FONT_WEIGHT_NORMAL);
        cr->set_font_size(12.0);
        cr->set_source_rgba(graphData->color[0], graphData->color[1],
                            graphData->color[2], 1.0);
        cr->save();
        cr->scale(1.0, -1.0);
        cr->move_to((*ditr - left) * horizScale,
                    -eventHeight - 3.0 * graph->_lineWidth - 2.0);
        cr->show_text(stringData->data[dataIndex]);
        cr->restore();
        cr->rectangle((*ditr - left) * horizScale - 1.5 * graph->_lineWidth,
                      eventHeight - 1.5 * graph->_lineWidth,
                      3.0 * graph->_lineWidth, 3.0 * graph->_lineWidth);
        cr->fill();
        cr->restore();
      }
  }
  
}
