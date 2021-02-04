#include "../../include/parser/parser_reg.h"

void parser_register_destroy(parser_register_t *reg) {
  while (reg->count > 0) {
    ufree(reg->identifiers[reg->count]);
    --reg->count;
  }
}

void parser_register_add(parser_register_t **reg, identifier_t *idf) {
  error_pos_t pos = {__FILE__, __func__, __LINE__};
  *reg = urealloc(&pos, *reg, sizeof(parser_register_t) + ((*reg)->count + 1) * sizeof(identifier_t));
  idf->id = (*reg)->count;
  (*reg)->identifiers[(*reg)->count++] = idf;
}

static int bins_wrapped(int to, int idf_id) {
  if (idf_id > to) {
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    error_raise(error_fatal, &pos, "Attempted to delete inexistent registry entry");
  }
  int lo = 0;
  int hi = to;
  int found_at = -1;
  while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (mid < idf_id) {
      lo = mid + 1;
    } else if (mid > idf_id) {
      hi = mid - 1;
    } else {
      found_at = mid;
      break;
    }
  }
  if (found_at == -1) {
    error_pos_t pos = {__FILE__, __func__, __LINE__};
    error_raise(error_fatal, &pos, "Attempted to delete inexistent registry entry");
  }
  return found_at;
}

void parser_register_remove(parser_register_t **reg, int idf_id) {
  int found_at = bins_wrapped((*reg)->count, idf_id);
  error_pos_t pos = {__FILE__, __func__, __LINE__};
  memmove((*reg)->identifiers + found_at,
          (*reg)->identifiers + found_at + 1,
          (--((*reg)->count) - found_at) * sizeof(identifier_t));  // shifts elements after found_at back by 1.
}

void parser_register_update(parser_register_t **reg, int idf_id, char *newvalue) {
  // ufree((*reg)->identifiers[bins_wrapped((*reg)->count, idf_id)]->value);
  (*reg)->identifiers[bins_wrapped((*reg)->count, idf_id)]->value = newvalue;
}

int parser_register_contains(parser_register_t **reg, const char *idf) {
  for (int i = 0; i < (*reg)->count; ++i) {
    if (!strcmp(idf, (*reg)->identifiers[i]->name)) {
      return i;
    }
  }
  return -1;
}