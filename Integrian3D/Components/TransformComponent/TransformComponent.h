#pragma once

#include "../../EngineConstants.h"

namespace Integrian3D
{
	class TransformComponent final
	{
	public:
		TransformComponent();

		void SetLocalLocation(const Point3f& pos);
		void SetLocalScale(const Point3f& scale);
		void SetLocalAngle(const Point3f& angleRad);

		const Point3f& GetWorldLocation() const { return WorldLocation; }
		Point3f GetLocalLocation() const { return Point3f{ Transformation(0,3), Transformation(1,3), Transformation(2,3) }; }

		const Point3f& GetWorldScale() const { return WorldScale; }
		const Point3f& GetLocalScale() const { return LocalScale; }

		const Point3f& GetWorldAngle() const { return WorldAngle; }
		const Point3f& GetLocalAngle() const { return LocalAngle; }

		bool bShouldRecalculateTransform;
		bool bShouldRecalculateWorldData;

	private:
		Matrix4f Transformation;

		Point3f WorldLocation;

		Point3f WorldScale;
		Point3f LocalScale;

		Point3f WorldAngle; /* In Radians */
		Point3f LocalAngle; /* In Radians */
	};
}