#pragma once

#define DECLARE_LOG_CATEGORY(Category) struct __LogCategory##Category final \
														{ \
															 const char* CategoryName = #Category; \
														};