
`pattern_formatter-inl.h`

```c++
template<typename Padder>
SPDLOG_INLINE void pattern_formatter::handle_flag_(char flag, details::padding_info padding)
{
    // process custom flags
    auto it = custom_handlers_.find(flag);
    if (it != custom_handlers_.end())
    {
        auto custom_handler = it->second->clone();
        custom_handler->set_padding_info(padding);
        formatters_.push_back(std::move(custom_handler));
        return;
    }

    // process built-in flags
    switch (flag)
    {
    case ('+'): // default formatter
        formatters_.push_back(details::make_unique<details::full_formatter>(padding));
        break;

    case 'n': // logger name
        formatters_.push_back(details::make_unique<details::name_formatter<Padder>>(padding));
        break;

    case 'l': // level
        formatters_.push_back(details::make_unique<details::level_formatter<Padder>>(padding));
        break;

    case 'L': // short level
        formatters_.push_back(details::make_unique<details::short_level_formatter<Padder>>(padding));
        break;

    case ('t'): // thread id
        formatters_.push_back(details::make_unique<details::t_formatter<Padder>>(padding));
        break;

    case ('v'): // the message text
        formatters_.push_back(details::make_unique<details::v_formatter<Padder>>(padding));
        break;

    case ('a'): // weekday
        formatters_.push_back(details::make_unique<details::a_formatter<Padder>>(padding));
        break;

    case ('A'): // short weekday
        formatters_.push_back(details::make_unique<details::A_formatter<Padder>>(padding));
        break;

    case ('b'):
    case ('h'): // month
        formatters_.push_back(details::make_unique<details::b_formatter<Padder>>(padding));
        break;

    case ('B'): // short month
        formatters_.push_back(details::make_unique<details::B_formatter<Padder>>(padding));
        break;

    case ('c'): // datetime
        formatters_.push_back(details::make_unique<details::c_formatter<Padder>>(padding));
        break;

    case ('C'): // year 2 digits
        formatters_.push_back(details::make_unique<details::C_formatter<Padder>>(padding));
        break;

    case ('Y'): // year 4 digits
        formatters_.push_back(details::make_unique<details::Y_formatter<Padder>>(padding));
        break;

    case ('D'):
    case ('x'): // datetime MM/DD/YY
        formatters_.push_back(details::make_unique<details::D_formatter<Padder>>(padding));
        break;

    case ('m'): // month 1-12
        formatters_.push_back(details::make_unique<details::m_formatter<Padder>>(padding));
        break;

    case ('d'): // day of month 1-31
        formatters_.push_back(details::make_unique<details::d_formatter<Padder>>(padding));
        break;

    case ('H'): // hours 24
        formatters_.push_back(details::make_unique<details::H_formatter<Padder>>(padding));
        break;

    case ('I'): // hours 12
        formatters_.push_back(details::make_unique<details::I_formatter<Padder>>(padding));
        break;

    case ('M'): // minutes
        formatters_.push_back(details::make_unique<details::M_formatter<Padder>>(padding));
        break;

    case ('S'): // seconds
        formatters_.push_back(details::make_unique<details::S_formatter<Padder>>(padding));
        break;

    case ('e'): // milliseconds
        formatters_.push_back(details::make_unique<details::e_formatter<Padder>>(padding));
        break;

    case ('f'): // microseconds
        formatters_.push_back(details::make_unique<details::f_formatter<Padder>>(padding));
        break;

    case ('F'): // nanoseconds
        formatters_.push_back(details::make_unique<details::F_formatter<Padder>>(padding));
        break;

    case ('E'): // seconds since epoch
        formatters_.push_back(details::make_unique<details::E_formatter<Padder>>(padding));
        break;

    case ('p'): // am/pm
        formatters_.push_back(details::make_unique<details::p_formatter<Padder>>(padding));
        break;

    case ('r'): // 12 hour clock 02:55:02 pm
        formatters_.push_back(details::make_unique<details::r_formatter<Padder>>(padding));
        break;

    case ('R'): // 24-hour HH:MM time
        formatters_.push_back(details::make_unique<details::R_formatter<Padder>>(padding));
        break;

    case ('T'):
    case ('X'): // ISO 8601 time format (HH:MM:SS)
        formatters_.push_back(details::make_unique<details::T_formatter<Padder>>(padding));
        break;

    case ('z'): // timezone
        formatters_.push_back(details::make_unique<details::z_formatter<Padder>>(padding));
        break;

    case ('P'): // pid
        formatters_.push_back(details::make_unique<details::pid_formatter<Padder>>(padding));
        break;

    case ('^'): // color range start
        formatters_.push_back(details::make_unique<details::color_start_formatter>(padding));
        break;

    case ('$'): // color range end
        formatters_.push_back(details::make_unique<details::color_stop_formatter>(padding));
        break;

    case ('@'): // source location (filename:filenumber)
        formatters_.push_back(details::make_unique<details::source_location_formatter<Padder>>(padding));
        break;

    case ('s'): // short source filename - without directory name
        formatters_.push_back(details::make_unique<details::short_filename_formatter<Padder>>(padding));
        break;

    case ('g'): // full source filename
        formatters_.push_back(details::make_unique<details::source_filename_formatter<Padder>>(padding));
        break;

    case ('#'): // source line number
        formatters_.push_back(details::make_unique<details::source_linenum_formatter<Padder>>(padding));
        break;

    case ('!'): // source funcname
        formatters_.push_back(details::make_unique<details::source_funcname_formatter<Padder>>(padding));
        break;

    case ('%'): // % char
        formatters_.push_back(details::make_unique<details::ch_formatter>('%'));
        break;

    case ('u'): // elapsed time since last log message in nanos
        formatters_.push_back(details::make_unique<details::elapsed_formatter<Padder, std::chrono::nanoseconds>>(padding));
        break;

    case ('i'): // elapsed time since last log message in micros
        formatters_.push_back(details::make_unique<details::elapsed_formatter<Padder, std::chrono::microseconds>>(padding));
        break;

    case ('o'): // elapsed time since last log message in millis
        formatters_.push_back(details::make_unique<details::elapsed_formatter<Padder, std::chrono::milliseconds>>(padding));
        break;

    case ('O'): // elapsed time since last log message in seconds
        formatters_.push_back(details::make_unique<details::elapsed_formatter<Padder, std::chrono::seconds>>(padding));
        break;

    default: // Unknown flag appears as is
        auto unknown_flag = details::make_unique<details::aggregate_formatter>();

        if (!padding.truncate_)
        {
            unknown_flag->add_ch('%');
            unknown_flag->add_ch(flag);
            formatters_.push_back((std::move(unknown_flag)));
        }
        // fix issue #1617 (prev char was '!' and should have been treated as funcname flag instead of truncating flag)
        // spdlog::set_pattern("[%10!] %v") => "[      main] some message"
        // spdlog::set_pattern("[%3!!] %v") => "[mai] some message"
        else
        {
            padding.truncate_ = false;
            formatters_.push_back(details::make_unique<details::source_funcname_formatter<Padder>>(padding));
            unknown_flag->add_ch(flag);
            formatters_.push_back((std::move(unknown_flag)));
        }

        break;
    }
}
```