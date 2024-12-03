--#-package:0d4f8210# -- package signature --
-- Based on Urgent Advice dialog code created by CorinaMarie @ Simtropolis.

-- Be sure to have Urgent Advice enabled and load an established city tile.

delete_advice('67d1fb22')            -- It won't exist yet, but this insures the record is cleared.
a = create_advice_safety('67d1fb22') -- Doesn't really matter which department, but it will in an advanced version.
a.trigger = "1"                      -- Always true.
a.title   = [[text@be34657d]]        -- This is how we use LText
a.message = [[text@be34657e]]        -- This is how we use LText

-- EOF
