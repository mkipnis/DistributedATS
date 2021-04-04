call insert_instrument('REPO_1', 'REPO');
call insert_update_ref_data('REPO_1', 'REPO', '{"security_type":"repo", "order_book_type":"inverted","tick_size":1000}');
call map_instrument_to_market('REPO_1', 'MARKET_Y');
