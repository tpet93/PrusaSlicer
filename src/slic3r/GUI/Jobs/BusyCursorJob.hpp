#ifndef BUSYCURSORJOB_HPP
#define BUSYCURSORJOB_HPP

#include "Job.hpp"

#include <wx/utils.h>

namespace Slic3r { namespace GUI {

struct CursorSetterRAII
{
    Job::Ctl &ctl;
    CursorSetterRAII(Job::Ctl &c) : ctl{c}
    {
        ctl.call_on_main_thread([] { wxBeginBusyCursor(); });
    }
    ~CursorSetterRAII()
    {
        ctl.call_on_main_thread([] { wxEndBusyCursor(); });
    }
};

template<class JobSubclass>
class BusyCursored: public Job {
    JobSubclass m_job;

public:
    template<class... Args>
    BusyCursored(Args &&...args) : m_job{std::forward<Args>(args)...}
    {}

    void process(Ctl &ctl) override
    {
        CursorSetterRAII cursor_setter{ctl};
        m_job.process(ctl);
    }

    void finalize(bool canceled, std::exception_ptr &eptr) override
    {
        m_job.finalize(canceled, eptr);
    }
};


}
}

#endif // BUSYCURSORJOB_HPP
