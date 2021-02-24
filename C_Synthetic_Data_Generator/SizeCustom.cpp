#include "SizeCustom.h"

void SizeCustom::GenerateTemplateFieldTypes() {
	myTemplate["data"] = new CharDataPoint("char", CharacterType::alphanumeric, buffer_size);
}
