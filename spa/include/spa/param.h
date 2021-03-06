/* Simple Plugin API
 * Copyright (C) 2017 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __SPA_PARAM_H__
#define __SPA_PARAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <spa/defs.h>
#include <spa/pod-utils.h>

struct spa_param;
#define SPA_TYPE__Param		SPA_TYPE_POD_OBJECT_BASE "Param"
#define SPA_TYPE_PARAM_BASE	SPA_TYPE__Param ":"

struct spa_param {
	struct spa_pod_object object;
};

static inline uint32_t
spa_pod_builder_push_param(struct spa_pod_builder *builder,
			   struct spa_pod_frame *frame,
			   uint32_t param_type)
{
	return spa_pod_builder_push_object(builder, frame, 0, param_type);
}

#define spa_pod_builder_param(b,param_type,...)			\
	spa_pod_builder_object(b, 0, param_type,		\
		##__VA_ARGS__)


#define spa_param_parse(param,...)				\
({								\
	struct spa_pod_parser __p;				\
	const struct spa_param *__param = param;		\
	spa_pod_parser_pod(&__p, &__param->object.pod);		\
	spa_pod_parser_get(&__p, "<", ##__VA_ARGS__, NULL);	\
})

#define SPA_PARAM_BODY_FOREACH(body, size, iter)							\
	for ((iter) = SPA_MEMBER((body), sizeof(struct spa_pod_object_body), struct spa_pod_prop);	\
	     (iter) < SPA_MEMBER((body), (size), struct spa_pod_prop);					\
	     (iter) = SPA_MEMBER((iter), SPA_ROUND_UP_N(SPA_POD_SIZE(iter), 8), struct spa_pod_prop))

#define SPA_PARAM_FOREACH(param, iter) \
	SPA_PARAM_BODY_FOREACH(&param->object.body, SPA_POD_BODY_SIZE(param), iter)

static inline int spa_param_fixate(struct spa_param *param)
{
	struct spa_pod_prop *prop;

	SPA_PARAM_FOREACH(param, prop)
		prop->body.flags &= ~SPA_POD_PROP_FLAG_UNSET;

	return SPA_RESULT_OK;
}

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __SPA_PARAM_H__ */
