
#pragma once

#include "array-view-geoarrow.hpp"
#include "array-view-wkb.hpp"
#include "array-view-wkt.hpp"

namespace geoarrow {

namespace {

// autogen factory start
ArrayView* create_view_point(struct ArrowSchema* schema, Meta& point_meta) {

    switch (point_meta.storage_type_) {
    case util::StorageType::FixedSizeList:
        return new PointArrayView(schema);

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.point");
    }

}

ArrayView* create_view_linestring(struct ArrowSchema* schema,
                                          Meta& linestring_meta) {
    Meta point_meta(schema->children[0]);


    switch (linestring_meta.storage_type_) {
    case util::StorageType::List:

            switch (point_meta.storage_type_) {
            case util::StorageType::FixedSizeList:
                return new LinestringArrayView(schema);

            default:
                throw Meta::ValidationError(
                    "Unsupported storage type for extension geoarrow.point");
            }

        break;

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.linestring");
    }

}

ArrayView* create_view_polygon(struct ArrowSchema* schema, Meta& polygon_meta) {
    Meta linestring_meta(schema->children[0]);
    Meta point_meta(schema->children[0]->children[0]);


    switch (polygon_meta.storage_type_) {
    case util::StorageType::List:

            switch (linestring_meta.storage_type_) {
            case util::StorageType::List:

                        switch (point_meta.storage_type_) {
                        case util::StorageType::FixedSizeList:
                            return new PolygonArrayView(schema);

                        default:
                            throw Meta::ValidationError(
                                "Unsupported storage type for extension geoarrow.point");
                        }

                break;

            default:
                throw Meta::ValidationError(
                    "Unsupported storage type for extension geoarrow.linestring");
            }

        break;

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.polygon");
    }

}

ArrayView* create_view_multipoint(struct ArrowSchema* schema,
                                          Meta& multi_meta, Meta& point_meta) {



    switch (multi_meta.storage_type_) {
    case util::StorageType::List:
        switch (point_meta.storage_type_) {
    case util::StorageType::FixedSizeList:
        return new CollectionArrayView<PointArrayView>(schema);

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.point");
    }
        break;

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.geometrycollection");
    }

}

ArrayView* create_view_multilinestring(struct ArrowSchema* schema,
                                               Meta& multi_meta,
                                               Meta& linestring_meta) {
    Meta point_meta(schema->children[0]->children[0]);


    switch (multi_meta.storage_type_) {
    case util::StorageType::List:
        switch (linestring_meta.storage_type_) {
    case util::StorageType::List:

        switch (point_meta.storage_type_) {
        case util::StorageType::FixedSizeList:
            return new CollectionArrayView<LinestringArrayView>(schema);

        default:
            throw Meta::ValidationError(
                "Unsupported storage type for extension geoarrow.point");
        }

        break;

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.linestring");
    }
        break;

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.geometrycollection");
    }

}

ArrayView* create_view_multipolygon(struct ArrowSchema* schema,
                                            Meta& multi_meta, Meta& polygon_meta) {
    Meta linestring_meta(schema->children[0]->children[0]);
    Meta point_meta(schema->children[0]->children[0]->children[0]);


    switch (multi_meta.storage_type_) {
    case util::StorageType::List:
        switch (polygon_meta.storage_type_) {
    case util::StorageType::List:

        switch (linestring_meta.storage_type_) {
        case util::StorageType::List:

                switch (point_meta.storage_type_) {
                case util::StorageType::FixedSizeList:
                    return new CollectionArrayView<PolygonArrayView>(schema);

                default:
                    throw Meta::ValidationError(
                        "Unsupported storage type for extension geoarrow.point");
                }

            break;

        default:
            throw Meta::ValidationError(
                "Unsupported storage type for extension geoarrow.linestring");
        }

        break;

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.polygon");
    }
        break;

    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.geometrycollection");
    }

}
// autogen factory end


ArrayView* create_view_collection(struct ArrowSchema* schema, Meta& multi_meta) {
    Meta child_meta(schema->children[0]);

    switch (child_meta.extension_) {
    case util::Extension::Point:
        return create_view_multipoint(schema, multi_meta, child_meta);

    case util::Extension::Linestring:
        return create_view_multilinestring(schema, multi_meta, child_meta);

    case util::Extension::Polygon:
        return create_view_multipolygon(schema, multi_meta, child_meta);
    default:
        throw Meta::ValidationError("Unsupported extension type for child of geoarrow.geometrycollection");
    }
}

ArrayView* create_view_wkb(struct ArrowSchema* schema, Meta& geoarrow_meta) {
    switch (geoarrow_meta.storage_type_) {
    case util::StorageType::Binary:
        return new WKBArrayView(schema);
    case util::StorageType::LargeBinary:
        return new LargeWKBArrayView(schema);
    case util::StorageType::FixedWidthBinary:
        return new FixedWidthWKBArrayView(schema);
    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.wkb");
    }
}

ArrayView* create_view_wkt(struct ArrowSchema* schema, Meta& geoarrow_meta) {
    switch (geoarrow_meta.storage_type_) {
    case util::StorageType::Binary:
    case util::StorageType::String:
        return new WKTArrayView(schema);
    case util::StorageType::LargeBinary:
    case util::StorageType::LargeString:
        return new LargeWKTArrayView(schema);
    default:
        throw Meta::ValidationError(
            "Unsupported storage type for extension geoarrow.wkt");
    }
}

} // anonymous namespace

ArrayView* create_view(struct ArrowSchema* schema) {
    // parse the schema and check that the structure is not unexpected
    // (e.g., the extension type and storage type are compatible and
    // there are not an unexpected number of children)
    Meta geoarrow_meta(schema);

    switch (geoarrow_meta.extension_) {
    case util::Extension::Point:
        return create_view_point(schema, geoarrow_meta);

    case util::Extension::Linestring:
        return create_view_linestring(schema, geoarrow_meta);

    case util::Extension::Polygon:
        return create_view_polygon(schema, geoarrow_meta);

    case util::Extension::MultiPoint:
    case util::Extension::MultiLinestring:
    case util::Extension::MultiPolygon:
    case util::Extension::GeometryCollection:
        return create_view_collection(schema, geoarrow_meta);

    case util::Extension::WKB:
        return create_view_wkb(schema, geoarrow_meta);

    case util::Extension::WKT:
        return create_view_wkt(schema, geoarrow_meta);

    default:
        throw Meta::ValidationError("Unsupported extension type");
    }
}

}
