
basic XML-Parser
 
supports:
- 'standard' XML (tag and value pairs, optionally attributes)
- self-closing tags (<tag/>), 
- processing instructions (<?xml...?>)
 
use XMLParser::parseXMLString() to parse a string into an XMLMessage
use XMLMessage and XMLMessage::AddTag() and the returned XMLTag& as well as it's toString() to construct an XML-message
