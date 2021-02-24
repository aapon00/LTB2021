#include "Size512.h"

void Size512::GenerateTemplateFieldTypes() {
	myTemplate["data"] = new CharDataPoint("char", CharacterType::alphanumeric, 512);
}
