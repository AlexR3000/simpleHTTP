#pragma once

namespace Http {
    enum class ParseError {
        None,
        InvalidAttribute,
        MissingAttribute,
    };
}